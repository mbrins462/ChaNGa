#ifndef NOCOOLING

/*
 * Cooling code originally written by James Wadsley, McMaster
 * University for GASOLINE.
 *
 * Updated for ChaNGa by Isaac Backus, University of Washington
 */
/*
 * Cooling functions for a planetary disk where the cooling time is
 * proportional to the orbital time.
 *
 * ALGORITHM:
 *
 * Integrating over a time step (see CoolIntegrateEnergyCode) gives:
 *      E(t+dt) = exp(-dt/tcool) (E(t) - PdV*tcool) + PdV*tcool
 * where:
 *      tcool = beta/omega
 * beta is a dimensionless cooling time (see CoolAddParams) and omega is the
 * approximate Keplerian angular velocity, calculated as:
 *      omega = sqrt(G*M/R**3)
 * where M is the total mass of all star particles and R is the distance to
 * the center of mass of star particles.
 *
 * NOTES:
 *
 * The convention is that Cool...() functions are public and cl..()
 * functions are private (which could be enforced if we move this to
 * C++).   The COOL class will contain data which is constant across
 * all processors, and will be contained in a Charm++ NodeGroup.
 * The clDerivsData structure will contain data that changes from
 * particle to particle.
 *
 * By convention, all cooling is done in CGS.  The header file defines macros
 * which convert between code and CGS units [cooling_planet.h].  For example,
 * to convert energy into CGS, do :
 *      E = CoolCodeEnergyToErgPerGm(cl, E);
 * Conversion factors are stored in a COOL struct (see clInitConstants)
 *
  For a list of parameters to set, see CoolAddParams in cooling_planet.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "cooling.h"

/**
 * @brief CoolInit initializes a COOL data structure for storing
 *      cooling constants and parameters
 * @return a blank COOL data structure with nTableRead = 0
 */
COOL *CoolInit( )
{
    COOL *cl;
    cl = (COOL *) malloc(sizeof(COOL));
    assert(cl!=NULL);
    return cl;
}

/**
 * Per thread initialization of cooling data.
 * @param cl Initialized COOL structure.
 */
clDerivsData *CoolDerivsInit(COOL *cl)
{
    clDerivsData *Data;

    assert(cl != NULL);
    Data = malloc(sizeof(clDerivsData));
    assert(Data != NULL);

    return Data;
}

/**
 * M.B. Checks to see if any nuclear mass fractions
 * or the predicted values are negative. If so,
 * we set them to zero.
 */
void cleanCoolParticle( COOLPARTICLE *cp) {

	if ( cp->HeFour < 0.0) { cp->HeFour = 0.0;}
	if ( cp->CTwelve < 0.0) { cp->CTwelve = 0.0;}
	if ( cp->OSixteen < 0.0) { cp->OSixteen = 0.0;}
	if ( cp->NeTwenty < 0.0) { cp->NeTwenty = 0.0;}
	if ( cp->MgTwentyFour < 0.0) { cp->MgTwentyFour = 0.0;}
	if ( cp->SiTwentyEight < 0.0) { cp->SiTwentyEight = 0.0;}
	if ( cp->SThirtyTwo < 0.0) { cp->SThirtyTwo = 0.0;}
	if ( cp->ArThirtySix < 0.0) { cp->ArThirtySix = 0.0;}
	if ( cp->CaForty < 0.0) { cp->CaForty = 0.0;}
	if ( cp->TiFortyFour < 0.0) { cp->TiFortyFour = 0.0;}
	if ( cp->CrFortyEight < 0.0) { cp->CrFortyEight = 0.0;}
	if ( cp->FeFiftyTwo < 0.0) { cp->FeFiftyTwo = 0.0;}
	if ( cp->NiFiftySix < 0.0) { cp->NiFiftySix = 0.0;}
	if ( cp->HeFourPred < 0.0) { cp->HeFourPred = 0.0;}
	if ( cp->CTwelvePred < 0.0) { cp->CTwelvePred = 0.0;}
	if ( cp->OSixteenPred < 0.0) { cp->OSixteenPred = 0.0;}
	if ( cp->NeTwentyPred < 0.0) { cp->NeTwentyPred = 0.0;}
	if ( cp->MgTwentyFourPred < 0.0) { cp->MgTwentyFourPred = 0.0;}
	if ( cp->SiTwentyEightPred < 0.0) { cp->SiTwentyEightPred = 0.0;}
	if ( cp->SThirtyTwoPred < 0.0) { cp->SThirtyTwoPred = 0.0;}
	if ( cp->ArThirtySixPred < 0.0) { cp->ArThirtySixPred = 0.0;}
	if ( cp->CaFortyPred < 0.0) { cp->CaFortyPred = 0.0;}
	if ( cp->TiFortyFourPred < 0.0) { cp->TiFortyFourPred = 0.0;}
	if ( cp->CrFortyEightPred < 0.0) { cp->CrFortyEightPred = 0.0;}
	if ( cp->FeFiftyTwoPred < 0.0) { cp->FeFiftyTwoPred = 0.0;}
	if ( cp->NiFiftySixPred < 0.0) { cp->NiFiftySixPred = 0.0;}

}

///Frees memory and deletes cl
void CoolFinalize(COOL *cl )
{
    free(cl);
}

/**
 * Deallocate memory for per-thread data.
 */
void CoolDerivsFinalize(clDerivsData *clData)
{
    free(clData);
}

/**
 * @brief clInitConstants sets physical constants and parameters
 *          for cooling
 * @param cl a COOL data structure (see CoolInit)
 * @param CoolParam an initialized COOLPARAM structure (see CoolAddParams)
 */
void clInitConstants(COOL *cl, double dGmPerCcUnit,
                     double dComovingGmPerCcUnit, double dErgPerGmUnit,
                     double dSecUnit, double dKpcUnit, COOLPARAM CoolParam)
{
    assert(cl!=NULL);
    /* Units */
    cl->dGmPerCcUnit = dGmPerCcUnit;
    cl->dComovingGmPerCcUnit = dComovingGmPerCcUnit;
    cl->dErgPerGmUnit = dErgPerGmUnit;
    cl->dSecUnit = dSecUnit;
    cl->dErgPerGmPerSecUnit = cl->dErgPerGmUnit / cl->dSecUnit;
    cl->diErgPerGmUnit = 1./dErgPerGmUnit;
    cl->dKpcUnit = dKpcUnit;
 
    cl->bDoBurning = CoolParam.bDoBurning;
    cl->bReadInitValFiles = CoolParam.bReadInitValFiles;

}

/**
 * @brief Calculates thermal energy from temperature and Y (deg. freedom/3)
 * @param Y_Total (deg. freedom)/3
 * @param T Temperature
 * @return Thermal (internal) energy per mass (CGS)
 */
double clThermalEnergy( COOLPARTICLE *cp, double dens, double T ) {
  double pressure, energyout, soundspeed;
  double abar; 
  double zbar;
  calAbarZbar( cp, &zbar, &abar);
  gethelmholtzeos_(&T, &dens, &abar, &zbar, &pressure, &energyout, &soundspeed);
  return energyout;	/*In ergs, of course.*/	

}
/**
 * @brief Calculates temperature from internal energy and Y (deg.freedom/3)
 * @param Y_Total (deg. freedom)/3
 * @param E Internal energy per mass (CGS units)
 * @return Temperature
 */
double clTemperature( COOLPARTICLE *cp, double dens, double E ) {
  /*ASSUMES THAT E AND RHO ARE IN CGS!!!*/  
  double temp = 1e7;
  double abar; 
  double zbar;
  double pressure, soundspeed;
  
  /*Get "Fermi" energy*/
  double dummytemp = 1e3;	
  double fermienergy;

  calAbarZbar( cp, &zbar, &abar);
  gethelmholtzeos_(&dummytemp, &dens, &abar, &zbar, &pressure, &fermienergy, &soundspeed);
   
  //printf("cooling_nuc.c clTemperature: Energy = %e, Fermi Energy = %e\n", E, fermienergy);//MB 
  /*Condition to prevent crashing when energy is mostly Fermi*/
  if(E < fermienergy) {
    temp = dummytemp;
  }	
  else	{
    geteosinversion_(&temp, &dens, &abar, &zbar, &pressure, &E, &soundspeed);  /*Get pressure, temp & soundspeed*/
  }

  //printf("cooling_nuc.c clTemperature: Temperature = %e\n",temp);//MB
  return temp;  /*In K, of course.*/	

}

/* Module Interface routines */

/**
 * @brief CoolAddParams Parses parameters and adds them to the COOLPARAM
 *          struct
 * @param CoolParam A COOLPARAM struct
 * @param prm The param struct
 *
 * The available parameters to be set are:
 *      dBetaCooling : Effective cooling time = t_cool * omega.  Also
 *          equal to 2*pi*(t_cool/T_orbit)
 *      dY_Total : (degrees of freedom)/3 for the gas
 *      dCoolingTmin : Minimum allowed temperature (in Kelvin)
 *      dCoolingTmax : Maximum allowed temperature (in Kelvin)
 */
void CoolAddParams( COOLPARAM *CoolParam, PRM prm ) {
  CoolParam->bDoBurning = 1;
  prmAddParam(prm,"bDoBurning",paramBool,&CoolParam->bDoBurning,sizeof(int),
                    "burning","enable/disable burning and keep mass fraction outputs");
  CoolParam->bReadInitValFiles = 1;
  prmAddParam(prm,"bReadInitValFiles",paramBool,&CoolParam->bReadInitValFiles,sizeof(int),
		    "mass frac files","enable/disable reading initial mass frac files");
}

/* Placeholder functions which just need to be defined to make
 * Sph.C and the compiler happy */

void CoolTableReadInfo( COOLPARAM *CoolParam, int cntTable, int *nTableColumns,
                        char *suffix ){}

void CoolTableRead( COOL *Cool, int nData, void *vData) {}

void CoolInitRatesTable( COOL *cl, COOLPARAM CoolParam ) 
{
  init_nuclear_burning_();
}
double CoolEdotInstantCode(COOL *cl, COOLPARTICLE *cp, double ECode,
                           double rhoCode, double ZMetal, double *posCode ) {}

/* Initialization Routines */

/**
 * @brief Initializes data and sets defaults for a cooling particle
 * @param cp a COOLPARTICLE structure (see cooling_planet.h)
 */
void CoolDefaultParticleData( COOLPARTICLE *cp ){}

/**
 * @brief Initializes data for a cooling particle, given a COOL struct
 * @param cl a COOL struct which has been initialized with constants defined
 * @param cp a cooling particle COOLPARTICLE struct
 * @param E Pointer to the particle's internal energy
 *
 * Initializes data for a cooling particle, given a COOL struct.
 * Sets Y_total and E in the cooling particle
 */
void CoolInitEnergyAndParticleData( COOL *cl, COOLPARTICLE *cp, double *E,
                                    double dDensity, double dTemp, double fMetal)
{
    // We only want to initialize mass fractions here
    // if there are no input tipsy files that we want
    // to read from
    if(cl->bReadInitValFiles) {
	//printf("cooling_nuc.c CoolInitEnergyAndParticleData: oSixteen_frac = %e\n", cp->OSixteen);//MB
	//printf("cooling_nuc.c CoolInitEnergyAndParticleData: siTwentyEight_frac = %e\n", cp->SiTwentyEight);//MB
	}
    else {
        cp->HeFour  = 0.04;
        cp->CTwelve  = 0.48;
        cp->OSixteen  = 0.48;
        cp->NeTwenty = 0.0;
        cp->MgTwentyFour = 0.0;
        cp->SiTwentyEight = 0.0;
        cp->SThirtyTwo  = 0.0;
        cp->ArThirtySix = 0.0;
        cp->CaForty = 0.0;
        cp->TiFortyFour = 0.0;
        cp->CrFortyEight = 0.0;
        cp->FeFiftyTwo = 0.0;
        cp->NiFiftySix = 0.0;
	}
    cp->HeFourPred  = cp->HeFour;
    cp->CTwelvePred  = cp->CTwelve;
    cp->OSixteenPred  = cp->OSixteen;
    cp->NeTwentyPred = cp->NeTwenty;
    cp->MgTwentyFourPred = cp->MgTwentyFour;
    cp->SiTwentyEightPred = cp->SiTwentyEight;
    cp->SThirtyTwoPred  = cp->SThirtyTwo;
    cp->ArThirtySixPred = cp->ArThirtySix;
    cp->CaFortyPred = cp->CaForty;
    cp->TiFortyFourPred = cp->TiFortyFour;
    cp->CrFortyEightPred = cp->CrFortyEight;
    cp->FeFiftyTwoPred = cp->FeFiftyTwo;
    cp->NiFiftySixPred = cp->NiFiftySix;
    //MB the following line is what is used in cooling_helm.c, maybe the density
    //is already in gm/cc?
    *E = clThermalEnergy(cp, dDensity, dTemp)*cl->diErgPerGmUnit;
    //*E = clThermalEnergy(cp, CodeDensityToComovingGmPerCc(cl, dDensity),dTemp)*cl->diErgPerGmUnit;
}

///A hold over from cosmology.  Sets the time which is available to all threads
void CoolSetTime( COOL *cl, double dTime, double z ) {
    cl->z = z;
    cl->dTime = dTime;
}


///Calculates the temperature of a COOLPARTICLE given its energy (in Code units)
double CoolCodeEnergyToTemperature( COOL *Cool, COOLPARTICLE *cp, double E, double dens,
                                    double fMetal) {
/*MB In cooling_helm.c, the following line takes dens as the density value and 
 * does not convert to cgs units. This line will be commented out to test if
 * this is where the temperature problem arises.
 */
    //return clTemperature(cp, CodeDensityToComovingGmPerCc(Cool, dens), E*Cool->dErgPerGmUnit);
    return clTemperature(cp, dens, E*Cool->dErgPerGmUnit);
}

/**
 * Updates a particle's internal energy based on cooling and PdV work.
 * Cooling is done according to the equation:
 *      Edot = -E/tcool + PdV
 * where PdV is the PdV work per time done on the particle and tcool
 * is given by:
 *      tcool = beta/omega
 * beta is a dimensionless cooling set by the runtime parameter dBetaCooling
 * and omega is the approximate keplerian orbital angular velocity
 * Solving the cooling equation over a time interval from 0 to t gives:
 *      E(t) = exp(-t/tcool) (E0 - PdV*tcool) + PdV*tcool
 *
 * The Energy (Ecode) is updated
 *
 * @brief CoolIntegrateEnergyCode Integrates a particle's internal energy
 * due to cooling and PdV work
 * @param cl The COOL structure containing constants/params for the simulation
 * @param clData [unused]
 * @param cp Cooling particle
 * @param ECode Particle internal energy in Code units
 * @param PdVCode PdV work per time in code units
 * @param rhoCode [unused]
 * @param ZMetal [Unused]
 * @param posCode particle position in code units
 * @param tStep Time step to integrate over, in seconds
 */


void CoolIntegrateEnergyCode(COOL *cl, clDerivsData *cData, COOLPARTICLE *cp, double *ECode, 
			     double PdVCode, double rhoCode, double ZMetal, double *posCode, double tStep ) 
{
  double gammaEff; 

  *ECode = CoolCodeEnergyToErgPerGm( cl, *ECode );	/*Convert ergs to GM units, found in the header.*/
  clIntegrateEnergy(cl,  cp, ECode, CoolCodeWorkToErgPerGmPerSec( cl, PdVCode ), 
		    CodeDensityToComovingGmPerCc(cl, rhoCode), tStep);
  *ECode = CoolErgPerGmToCodeEnergy(cl, *ECode);
  
}


void clIntegrateEnergy(COOL *cl, COOLPARTICLE *cp, double *E, 
		       double PdV, double rho, double tStep) {

  double mytime = cl->dTime;
  double dEdt,dE,Ein = *E,EMin;
  double t=0,dtused,dtnext,tstop = tStep*(1-1e-8),dtEst;

  double dens = rho;//*(cl)->dGmPerCcUnit;	/*Rho is NOT in code units; see CoolIntegrateEnergyCode*/
  
  if (tStep <= 0) return;

  *E = *E + PdV*tStep;
  //printf("cooling_nuc.c clIntegrateEnergy (First time): Energy = %e\n", E);//MB


  if(cl->bDoBurning) {
/* MB
 * The energy is already converted to Ergs/gram units in CoolIntegrateEnergyCode function above, so we shouldn't need
 * to convert it again when we pass as an argument in clTemperature in the next line. Will try to pass only *E as
 * the argument.
 */
  double temp_in = clTemperature(cp, dens, *E), temp_out;
//  double temp_in = clTemperature(cp, dens, *E*cl->dErgPerGmUnit), temp_out;
  
 // printf("cooling_nuc.c clIntegrateEnergy: temp_in = %e\n", temp_in); //MB 
  //if( temperature < 8e8 || temperature > 1e10) return;  // TODO make this adjustable
  double energy_in, energy_out, density_in, density_out;
  double heFour_massfrac_in, heFour_massfrac_out, cTwelve_massfrac_in, cTwelve_massfrac_out, oSixteen_massfrac_in, oSixteen_massfrac_out, 
    neTwenty_massfrac_in, neTwenty_massfrac_out,mgTwentyFour_massfrac_in, mgTwentyFour_massfrac_out,siTwentyEight_massfrac_in, siTwentyEight_massfrac_out,
    sThirtyTwo_massfrac_in, sThirtyTwo_massfrac_out, arThirtySix_massfrac_in, arThirtySix_massfrac_out,caForty_massfrac_in, caForty_massfrac_out,
    tiFortyFour_massfrac_in, tiFortyFour_massfrac_out,crFortyEight_massfrac_in, crFortyEight_massfrac_out,
    feFiftyTwoFiftyTwo_massfrac_in, feFiftyTwoFiftyTwo_massfrac_out, niFiftySix_massfrac_in, niFiftySix_massfrac_out;

  energy_in = *E;
  density_in = dens;

  heFour_massfrac_in = cp->HeFour;
  cTwelve_massfrac_in = cp->CTwelve;
  oSixteen_massfrac_in = cp->OSixteen;
  neTwenty_massfrac_in = cp->NeTwenty;
  mgTwentyFour_massfrac_in = cp->MgTwentyFour;
  siTwentyEight_massfrac_in = cp->SiTwentyEight;
  sThirtyTwo_massfrac_in = cp->SThirtyTwo;
  arThirtySix_massfrac_in = cp->ArThirtySix;
  caForty_massfrac_in = cp->CaForty;
  tiFortyFour_massfrac_in = cp->TiFortyFour;
  crFortyEight_massfrac_in = cp->CrFortyEight;
  feFiftyTwoFiftyTwo_massfrac_in = cp->FeFiftyTwo;
  niFiftySix_massfrac_in = cp->NiFiftySix;

  drive_burner_(&tStep, &temp_in, &temp_out, 
		&energy_in, &energy_out, &density_in, &density_out,
		&heFour_massfrac_in, &heFour_massfrac_out, 
		&cTwelve_massfrac_in, &cTwelve_massfrac_out, 
		&oSixteen_massfrac_in, &oSixteen_massfrac_out, 
		&neTwenty_massfrac_in, &neTwenty_massfrac_out, 
		&mgTwentyFour_massfrac_in, &mgTwentyFour_massfrac_out, 
		&siTwentyEight_massfrac_in, &siTwentyEight_massfrac_out, 
		&sThirtyTwo_massfrac_in, &sThirtyTwo_massfrac_out, 
		&arThirtySix_massfrac_in, &arThirtySix_massfrac_out, 
		&caForty_massfrac_in, &caForty_massfrac_out, 
		&tiFortyFour_massfrac_in, &tiFortyFour_massfrac_out, 
		&crFortyEight_massfrac_in, &crFortyEight_massfrac_out, 
		&feFiftyTwoFiftyTwo_massfrac_in, &feFiftyTwoFiftyTwo_massfrac_out, 
		&niFiftySix_massfrac_in, &niFiftySix_massfrac_out);

  // calculate duDelta in code time
  double duDelta = CoolSecondsToCodeTime( cl, tStep);
  
  cp->HeFourDot = (heFour_massfrac_out - heFour_massfrac_in)/duDelta;
  cp->CTwelveDot = (cTwelve_massfrac_out - cTwelve_massfrac_in)/duDelta;
  cp->OSixteenDot = (oSixteen_massfrac_out - oSixteen_massfrac_in)/duDelta;
  cp->NeTwentyDot = (neTwenty_massfrac_out - neTwenty_massfrac_in)/duDelta;
  cp->MgTwentyFourDot = (mgTwentyFour_massfrac_out - mgTwentyFour_massfrac_in)/duDelta;
  cp->SiTwentyEightDot = (siTwentyEight_massfrac_out - siTwentyEight_massfrac_in)/duDelta;
  cp->SThirtyTwoDot = (sThirtyTwo_massfrac_out - sThirtyTwo_massfrac_in)/duDelta;
  cp->ArThirtySixDot = (arThirtySix_massfrac_out - arThirtySix_massfrac_in)/duDelta;
  cp->CaFortyDot = (caForty_massfrac_out - caForty_massfrac_in)/duDelta;
  cp->TiFortyFourDot = (tiFortyFour_massfrac_out - tiFortyFour_massfrac_in)/duDelta;
  cp->CrFortyEightDot = (crFortyEight_massfrac_out - crFortyEight_massfrac_in)/duDelta;
  cp->FeFiftyTwoDot = (feFiftyTwoFiftyTwo_massfrac_out - feFiftyTwoFiftyTwo_massfrac_in)/duDelta;
  cp->NiFiftySixDot = (niFiftySix_massfrac_out - niFiftySix_massfrac_in)/duDelta;
  *E = energy_out;
  }
  else { 
#ifdef ENERGY_RELAX_DV
    /*printf("Doing Energy Relaxation\n");*/
    double Ecoolto = clThermalEnergy(cp, dens, 5e6);	//HARDCODED END RELAXATION TEMPERATURE
    /*printf("Dens %lf    Fermi %lf",dens,Efermi);*/
    *E = (*E - Ecoolto)*ENERGYMOD + Ecoolto;  
    //*E = Ecoolto;
#endif

    double Elowerlim = 0.5*clThermalEnergy(cp, dens, 1e3 );
    //printf("cooling_nuc.c clIntegrateEnergy (Second time): Elowerlim = %e, E = %e\n", Elowerlim, E);//MB
    *E = fmax( *E, Elowerlim);
    //printf("cooling_nuc.c clIntegrateEnergy (Third time): Energy = %e\n", E);//MB
  }
}

void CoolCodePressureOnDensitySoundSpeed( COOL *cl, COOLPARTICLE *cp,
    double uPred, double fDensity, double gamma, double gammam1,
    double *PoverRho, double *c) { 
  double rho = fDensity*cl->dGmPerCcUnit; 
  double energy = uPred*cl->dErgPerGmUnit; 
  double pressure; 
  double fermipressure; /*unecessary variable - we could just use pressure, but to keep things obvious...*/ 
  double soundspeed; 
  double temp = 1e7; /*Initial guess for temperature*/
  double abar; 
  double zbar;
  double dummytemp = 1e3; 
  double fermienergy; 
  calAbarZbar( cp, &zbar, &abar);
  gethelmholtzeos_(&dummytemp, &rho, &abar, &zbar, &fermipressure, &fermienergy, &soundspeed); 
  if (energy <= fermienergy) { 
     pressure = fermipressure; /*set pressure to fermi pressure*/ 
  } 
  else { 
     geteosinversion_(&temp, &rho, &abar, &zbar, &pressure, &energy, &soundspeed);  /*Get pressure, temp & soundspeed*/ 
  } 
  *PoverRho = pressure/rho*cl->diErgPerGmUnit; /*Calculate P/rho & convert*/ 
  *c = soundspeed/(cl->dKpcUnit*3.0857e21/cl->dSecUnit); 
}

void calAbarZbar( COOLPARTICLE *cp, double *zbar, double *abar) { 	

  *abar = 1./(cp->HeFour/4. + cp->CTwelve/12. + cp->OSixteen/16. + cp->NeTwenty/20. +
                    cp->MgTwentyFour/24. + cp->SiTwentyEight/28. + cp->SThirtyTwo/32. + cp->ArThirtySix/36. +
                    cp->CaForty/40. + cp->TiFortyFour/44. + cp->CrFortyEight/48. + cp->FeFiftyTwo/52. + cp->NiFiftySix/56.); 
  *zbar = 1./(cp->HeFour/2. + cp->CTwelve/6. + cp->OSixteen/8. + cp->NeTwenty/10. +
		    cp->MgTwentyFour/12. + cp->SiTwentyEight/14. + cp->SThirtyTwo/16. + cp->ArThirtySix/18. +
		    cp->CaForty/20. + cp->TiFortyFour/22. + cp->CrFortyEight/24. + cp->FeFiftyTwo/26. + cp->NiFiftySix/28.); 
}

#endif /* NOCOOLING */
