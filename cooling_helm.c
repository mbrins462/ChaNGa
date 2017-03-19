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

}

/**
 * @brief Calculates thermal energy from temperature and Y (deg. freedom/3)
 * @param Y_Total (deg. freedom)/3
 * @param T Temperature
 * @return Thermal (internal) energy per mass (CGS)
 */
double clThermalEnergy( COOLPARTICLE *cp, double dens, double T ) {
  double pressure, energyout, soundspeed;
  double abar = 13.714285714285715;
  double zbar = abar/2.0;
  gethelmholtzeos_(&T, &dens, &abar, &zbar, &pressure, &energyout, &soundspeed);
  return energyout;	/*In ergs, of course.*/	

}
/**
 * @brief Calculates temperature from internal energy and Y (deg.freedom/3)
 * @param Y_Total (deg. freedom)/3
 * @param E Internal energy per mass (CGS units)
 * @return Temperature
 */
double clTemperature( double dens, double E ) {
  /*ASSUMES THAT E AND RHO ARE IN CGS!!!*/  
  double temp = 1e7;
  double abar = 13.714285714285715;
  double zbar = abar/2.0;
  double pressure, soundspeed;
  
  /*Get "Fermi" energy*/
  double dummytemp = 1e3;	
  double fermienergy;

  gethelmholtzeos_(&dummytemp, &dens, &abar, &zbar, &pressure, &fermienergy, &soundspeed);
   
//  printf("cooling_helm.c clTemperature: Energy = %e, Fermi Energy = %e\n", E, fermienergy);//MB
  /*Condition to prevent crashing when energy is mostly Fermi*/
  if(E < fermienergy) {
    temp = dummytemp;
  }	
  else	{
    geteosinversion_(&temp, &dens, &abar, &zbar, &pressure, &E, &soundspeed);  /*Get pressure, temp & soundspeed*/
  }

//  printf("cooling_helm.c clTemperature: Temperature = %e\n", temp);//MB
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
void CoolAddParams( COOLPARAM *CoolParam, PRM prm ) {}

/* Placeholder functions which just need to be defined to make
 * Sph.C and the compiler happy */

void CoolTableReadInfo( COOLPARAM *CoolParam, int cntTable, int *nTableColumns,
                        char *suffix ){}

void CoolTableRead( COOL *Cool, int nData, void *vData) {}

void CoolInitRatesTable( COOL *cl, COOLPARAM CoolParam ) 
{
  initializehelmholtz_();
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
                                    double dDensity, double dTemp, double fMetal)//, double *CarbonMF, double *OxygenMF)
{
    *E = clThermalEnergy(cp, dDensity,dTemp)*cl->diErgPerGmUnit;
/* FIXME
 * This function is called in Sph.C and doesn't have the CarbonMF OxygenMF arguments given, 
 * so I've commented out the arguments above.
 */
//    *CarbonMF = 0.35;
//    *OxygenMF = 0.65;
}

///A hold over from cosmology.  Sets the time which is available to all threads
void CoolSetTime( COOL *cl, double dTime, double z ) {
    cl->z = z;
    cl->dTime = dTime;
}


///Calculates the temperature of a COOLPARTICLE given its energy (in Code units)
double CoolCodeEnergyToTemperature( COOL *Cool, COOLPARTICLE *cp, double E, double dens,
                                    double fMetal) {
    return clTemperature(dens, E*Cool->dErgPerGmUnit);
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
  double rho = rhoCode*cl->dGmPerCcUnit; 
  double gammaEff; 

  *ECode = CoolCodeEnergyToErgPerGm( cl, *ECode );	/*Convert ergs to GM units, found in the header.*/
  clIntegrateEnergy(cl,  ECode, CoolCodeWorkToErgPerGmPerSec( cl, PdVCode ), 
		    CodeDensityToComovingGmPerCc(cl, rhoCode), tStep);
  *ECode = CoolErgPerGmToCodeEnergy(cl, *ECode);
  
}

void clIntegrateEnergy(COOL *cl, double *E, 
		       double PdV, double rho, double tStep) {

  double mytime = cl->dTime;
  double dEdt,dE,Ein = *E,EMin;
  double t=0,dtused,dtnext,tstop = tStep*(1-1e-8),dtEst;

  double dens = rho;//*(cl)->dGmPerCcUnit;	/*Rho is NOT in code units; see CoolIntegrateEnergyCode*/
  
  if (tStep <= 0) return;

  *E = *E + PdV*tStep;
//  printf("cooling_helm.c clIntegrateEnergy (First Time): Energy = %e\n", E);//MB

#ifdef ENERGY_RELAX_DV
  /*printf("Doing Energy Relaxation\n");*/
  double Ecoolto = clThermalEnergy( 0, dens, 5e6);	//HARDCODED END RELAXATION TEMPERATURE
  /*printf("Dens %lf    Fermi %lf",dens,Efermi);*/
  *E = (*E - Ecoolto)*ENERGYMOD + Ecoolto;  
  //*E = Ecoolto;
#endif

  double Elowerlim = 0.5*clThermalEnergy( 0, dens, 1e3 );
//  printf("cooling_helm.c clIntegrateEnergy (Second Time): Elowerlim = %e, E = %e\n",Elowerlim, E);//MB
  *E = fmax( *E, Elowerlim);
//  printf("cooling_helm.c clIntegrateEnergy (Third Time): Energy = %e\n", E);//MB

/* The next lines of code are to test the difference between the temperature output
 * here in cooling_helm.c and that in cooling_nuc.c that we use as input for our
 * drive_burner subroutine. It is not used for anything other than this test and 
 * the print out.
 */
  double temp_test = clTemperature( dens, *E);
//  printf("cooling_helm.c clIntegrateEnergy: TEST TEMPERATURE = %e\n",temp_test);//MB
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
  double abar = 13.714285714285715; 
  double zbar = 0.5*abar; 
  double dummytemp = 1e3; 
  double fermienergy; 
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


#endif /* NOCOOLING */
