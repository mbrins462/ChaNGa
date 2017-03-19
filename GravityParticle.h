/** @file GravityParticle.h
 * Defines the fundamental particle data structures.
 */
#ifndef GRAVITYPARTICLE_H
#define GRAVITYPARTICLE_H

#include "cooling.h"

#include "SFC.h"
#include <vector>

/// @brief Object to bookkeep a Bucket Walk.
class BucketGravityRequest {
public:
		
	int finished;

	BucketGravityRequest(unsigned int bucketSize = 0) : finished(0) {
	}
	
};

/// @brief Information needed to calculate gravity
///
/// This is used in the CacheParticle class since it contains only the
/// information that an external procesor needs to calculate gravity.
///
class ExternalGravityParticle {
 public:

  double mass;
  double soft;
  Vector3D<double> position;

  void pup(PUP::er &p) {
    p | position;
    p | mass;
    p | soft;
  }
};

/// @brief Extra data needed for SPH
class extraSPHData {
 private:
    double _u;			/* Internal Energy */
    double _fMetals;		/* Metalicity */
    double _fMFracOxygen;	/* Oxygen mass fraction  */
    double _fMFracIron;		/* Iron mass fraction  */
    double _fESNrate;		/* SN energy rate  */
    double _fTimeCoolIsOffUntil;/* time cooling is turned back on */
    Vector3D<double> _vPred;	/* Predicted velocities for velocity
				   dependent forces */
    double _uPred;		/* Predicted internal energy */
    double _divv;		/* Diverence of the velocity */
    Vector3D<double> _curlv;	/* Curl of the velocity */
    double _mumax;		/* */
    double _PdV;
    double _c;			/* Speed of Sound */
    double _PoverRho2;		/* Pressure/rho^2 */
    double _BalsaraSwitch;	/* Pressure/rho^2 */
    double _fBallMax;		/* Radius for inverse neighbor finding */
#ifndef COOLING_NONE
    double _uDot;		/* Rate of change of u, for
				   predicting u */
    COOLPARTICLE _CoolParticle;	/* Abundances and any other cooling
				   internal variables */
#endif
    
 public:
    inline double& u() {return _u;}
    inline double& fMetals() {return _fMetals;}
    inline double& fMFracOxygen() {return _fMFracOxygen;}
    inline double& fMFracIron() {return _fMFracIron;}
    inline double& fESNrate() {return _fESNrate;}
    inline double& fTimeCoolIsOffUntil() {return _fTimeCoolIsOffUntil;}
    inline Vector3D<double>& vPred() {return _vPred;}
    inline double& uPred() {return _uPred;}
    inline double& divv() {return _divv;}
    inline Vector3D<double>& curlv() {return _curlv;}
    inline double& mumax() {return _mumax;}
    inline double& PdV() {return _PdV;}
    inline double& c() {return _c;}
#ifdef COOLING_NUC
    inline double& HeFour() {return _CoolParticle.HeFour;}
    inline double& CTwelve() {return _CoolParticle.CTwelve;}
    inline double& OSixteen() {return _CoolParticle.OSixteen;}
    inline double& NeTwenty() {return _CoolParticle.NeTwenty;}
    inline double& MgTwentyFour() {return _CoolParticle.MgTwentyFour;}
    inline double& SiTwentyEight() {return _CoolParticle.SiTwentyEight;}
    inline double& SThirtyTwo()  {return _CoolParticle.SThirtyTwo;}
    inline double& ArThirtySix() {return _CoolParticle.ArThirtySix;}
    inline double& CaForty() {return _CoolParticle.CaForty;}
    inline double& TiFortyFour() {return _CoolParticle.TiFortyFour;}
    inline double& CrFortyEight() {return _CoolParticle.CrFortyEight;}
    inline double& FeFiftyTwo() {return _CoolParticle.FeFiftyTwo;}
    inline double& NiFiftySix() {return _CoolParticle.NiFiftySix;}
    inline double& HeFourDot() {return _CoolParticle.HeFourDot;}
    inline double& CTwelveDot() {return _CoolParticle.CTwelveDot;}
    inline double& OSixteenDot() {return _CoolParticle.OSixteenDot;}
    inline double& NeTwentyDot() {return _CoolParticle.NeTwentyDot;}
    inline double& MgTwentyFourDot() {return _CoolParticle.MgTwentyFourDot;}
    inline double& SiTwentyEightDot() {return _CoolParticle.SiTwentyEightDot;}
    inline double& SThirtyTwoDot()  {return _CoolParticle.SThirtyTwoDot;}
    inline double& ArThirtySixDot() {return _CoolParticle.ArThirtySixDot;}
    inline double& CaFortyDot() {return _CoolParticle.CaFortyDot;}
    inline double& TiFortyFourDot() {return _CoolParticle.TiFortyFourDot;}
    inline double& CrFortyEightDot() {return _CoolParticle.CrFortyEightDot;}
    inline double& FeFiftyTwoDot() {return _CoolParticle.FeFiftyTwoDot;}
    inline double& NiFiftySixDot() {return _CoolParticle.NiFiftySixDot;}
    inline double& HeFourPred() {return _CoolParticle.HeFourPred;}
    inline double& CTwelvePred() {return _CoolParticle.CTwelvePred;}
    inline double& OSixteenPred() {return _CoolParticle.OSixteenPred;}
    inline double& NeTwentyPred() {return _CoolParticle.NeTwentyPred;}
    inline double& MgTwentyFourPred() {return _CoolParticle.MgTwentyFourPred;}
    inline double& SiTwentyEightPred() {return _CoolParticle.SiTwentyEightPred;}
    inline double& SThirtyTwoPred()  {return _CoolParticle.SThirtyTwoPred;}
    inline double& ArThirtySixPred() {return _CoolParticle.ArThirtySixPred;}
    inline double& CaFortyPred() {return _CoolParticle.CaFortyPred;}
    inline double& TiFortyFourPred() {return _CoolParticle.TiFortyFourPred;}
    inline double& CrFortyEightPred() {return _CoolParticle.CrFortyEightPred;}
    inline double& FeFiftyTwoPred() {return _CoolParticle.FeFiftyTwoPred;}
    inline double& NiFiftySixPred() {return _CoolParticle.NiFiftySixPred;}
#endif
    inline double& PoverRho2() {return _PoverRho2;}
    inline double& BalsaraSwitch() {return _BalsaraSwitch;}
    inline double& fBallMax() {return _fBallMax;}
#ifndef COOLING_NONE
    inline double& uDot() {return _uDot;}
    inline COOLPARTICLE& CoolParticle() {return _CoolParticle;}
#endif
    void pup(PUP::er &p) {
	p | _u;
	p | _fMetals;
	p | _fMFracIron;
	p | _fMFracOxygen;
	p | _fESNrate;
	p | _fTimeCoolIsOffUntil;
	p | _vPred;
	p | _uPred;
	p | _divv;
	p | _curlv;
	p | _mumax;
	p | _PdV;
	p | _c;
	p | _PoverRho2;
	p | _BalsaraSwitch;
	p | _fBallMax;
#ifndef COOLING_NONE
	p | _uDot;
	p((char *) &_CoolParticle, sizeof(_CoolParticle)); /* PUPs as bytes */
#endif
	}
    };

/// @brief Extra data needed for Stars
class extraStarData 
{
 private:
    double _fMetals;		/* Metalicity */
    double _fTimeForm;		/* Formation time */
    double _fMassForm;		/* Formation mass */
    double _fESNrate;		/* SN energy rate  */
    double _fNSN;               /* number of SN exploding */
    double _fMSN;               /* mass of feedback ejecta */
    double _fMFracOxygen;	/* Oxygen mass fraction  */
    double _fMFracIron;		/* Iron mass fraction  */
    double _fSNMetals;          /* Ejected metals from feedback */
    double _fMOxygenOut;        /* Ejected oxygen */
    double _fMIronOut;          /* Ejected iron */
    int64_t _iGasOrder;		/* Gas from which this star formed */
 public:
    inline double& fMetals() {return _fMetals;}
    inline double& fTimeForm() {return _fTimeForm;}
    inline double& fMassForm() {return _fMassForm;}
    inline double& fESNrate() {return _fESNrate;}
    inline double& fNSN() {return _fNSN;}
    inline double& fMSN() {return _fMSN;}
    inline double& fMFracOxygen() {return _fMFracOxygen;}
    inline double& fMFracIron() {return _fMFracIron;}
    inline double& fMIronOut() {return _fMIronOut;}
    inline double& fMOxygenOut() {return _fMOxygenOut;}
    inline double& fSNMetals() {return _fSNMetals;}
    inline int64_t& iGasOrder() {return _iGasOrder;}
    void pup(PUP::er &p) {
	p | _fMetals;
	p | _fTimeForm;
	p | _fMassForm;
	p | _fESNrate;
	p | _fNSN;    
	p | _fMSN;    
	p | _fMFracOxygen;
	p | _fMFracIron;
	p | _fSNMetals;
	p | _fMOxygenOut;
	p | _fMIronOut;
	p | _iGasOrder;
	}
    };

class GravityParticle;
int TYPETest(const GravityParticle *a, unsigned int b);

class ExternalSmoothParticle;

/// @brief Fundamental type for a particle
///
/// This class contains everything that a "dark matter" particle needs.
/// Other classes of particles require this plus an "extra data" class.

class GravityParticle : public ExternalGravityParticle {
public:
	SFC::Key key;
	Vector3D<double> velocity;
	Vector3D<double> treeAcceleration;
	double potential;
	double dtGrav;
	double fBall;
	double fDensity;
	int64_t iOrder;		/* input order of particles */
        int rung;  ///< the current rung (greater means faster)
	unsigned int iType;	// Bitmask to hold particle type information
#ifdef CHANGESOFT
	double fSoft0;
#endif
#ifdef NEED_DT
	double dt;
#endif
	void *extraData;	/* SPH or Star particle data */

#if COSMO_STATS > 1
	double intcellmass;
	double intpartmass;
	double extcellmass;
	double extpartmass;
#endif

        double interMass;
	
        GravityParticle(SFC::Key k) : ExternalGravityParticle() {
            key = k;
            }
        GravityParticle() : ExternalGravityParticle() {
            }

	/// @brief Used to sort the particles into tree order.
	inline bool operator<(const GravityParticle& p) const {
		return key < p.key;
	}

	void pup(PUP::er &p) {
          ExternalGravityParticle::pup(p);
          p | key;
          p | velocity;
	  p | treeAcceleration;
	  p | fDensity;
	  p | fBall;
          p | iOrder;
          p | rung;
	  p | iType;
#ifdef CHANGESOFT
	  p | fSoft0;
#endif
#ifdef NEED_DT
	  p | dt;
#endif
        }

// Debugging macros for the extra data fields.
// To enable, define GP_DEBUG_EXTRAS

#define GP_DEBUG_EXTRAS

#ifdef GP_DEBUG_EXTRAS
#define IMAGAS CkAssert(isGas())
#define IMASTAR CkAssert(isStar())
#else
#define IMAGAS
#define IMASTAR
#endif

	// Access SPH quantities
	/// @brief Get quantities needed for SPH smooths.
	ExternalSmoothParticle getExternalSmoothParticle();
	inline double& u() { IMAGAS; return (((extraSPHData*)extraData)->u());}
	inline double& fMetals() { IMAGAS; return (((extraSPHData*)extraData)->fMetals());}
	inline double& fMFracOxygen() {IMAGAS; return (((extraSPHData*)extraData)->fMFracOxygen());}
	inline double& fMFracIron() {IMAGAS; return (((extraSPHData*)extraData)->fMFracIron());}
	inline double& fESNrate() {IMAGAS; return (((extraSPHData*)extraData)->fESNrate());}
	inline double& fTimeCoolIsOffUntil() {IMAGAS; return (((extraSPHData*)extraData)->fTimeCoolIsOffUntil());}
	inline Vector3D<double>& vPred() { IMAGAS; return (((extraSPHData*)extraData)->vPred());}
	inline double& uPred() {IMAGAS;  return (((extraSPHData*)extraData)->uPred());}
	inline double& divv() { IMAGAS; return (((extraSPHData*)extraData)->divv());}
	inline Vector3D<double>& curlv() { IMAGAS; return (((extraSPHData*)extraData)->curlv());}
	inline double& mumax() { IMAGAS; return (((extraSPHData*)extraData)->mumax());}
	inline double& PdV() { IMAGAS; return (((extraSPHData*)extraData)->PdV());}
	inline double& c() { IMAGAS; return (((extraSPHData*)extraData)->c());}
#ifdef COOLING_NUC
	inline double& HeFour() { IMAGAS; return (((extraSPHData*)extraData)->HeFour());}
	inline double& CTwelve() { IMAGAS; return (((extraSPHData*)extraData)->CTwelve());}
	inline double& OSixteen() { IMAGAS; return (((extraSPHData*)extraData)->OSixteen());}
        inline double& NeTwenty() { IMAGAS; return (((extraSPHData*)extraData)->NeTwenty());}
        inline double& MgTwentyFour() { IMAGAS; return (((extraSPHData*)extraData)->MgTwentyFour());}
        inline double& SiTwentyEight() { IMAGAS; return (((extraSPHData*)extraData)->SiTwentyEight());}
        inline double& SThirtyTwo()  { IMAGAS; return (((extraSPHData*)extraData)->SThirtyTwo());}
        inline double& ArThirtySix() { IMAGAS; return (((extraSPHData*)extraData)->ArThirtySix());}
        inline double& CaForty() { IMAGAS; return (((extraSPHData*)extraData)->CaForty());}
        inline double& TiFortyFour() { IMAGAS; return (((extraSPHData*)extraData)->TiFortyFour());}
        inline double& CrFortyEight() { IMAGAS; return (((extraSPHData*)extraData)->CrFortyEight());}
        inline double& FeFiftyTwo() { IMAGAS; return (((extraSPHData*)extraData)->FeFiftyTwo());}
        inline double& NiFiftySix() { IMAGAS; return (((extraSPHData*)extraData)->NiFiftySix());}
	inline double& HeFourDot() { IMAGAS; return (((extraSPHData*)extraData)->HeFourDot());}
	inline double& CTwelveDot() { IMAGAS; return (((extraSPHData*)extraData)->CTwelveDot());}
	inline double& OSixteenDot() { IMAGAS; return (((extraSPHData*)extraData)->OSixteenDot());}
        inline double& NeTwentyDot() { IMAGAS; return (((extraSPHData*)extraData)->NeTwentyDot());}
        inline double& MgTwentyFourDot() { IMAGAS; return (((extraSPHData*)extraData)->MgTwentyFourDot());}
        inline double& SiTwentyEightDot() { IMAGAS; return (((extraSPHData*)extraData)->SiTwentyEightDot());}
        inline double& SThirtyTwoDot()  { IMAGAS; return (((extraSPHData*)extraData)->SThirtyTwoDot());}
        inline double& ArThirtySixDot() { IMAGAS; return (((extraSPHData*)extraData)->ArThirtySixDot());}
        inline double& CaFortyDot() { IMAGAS; return (((extraSPHData*)extraData)->CaFortyDot());}
        inline double& TiFortyFourDot() { IMAGAS; return (((extraSPHData*)extraData)->TiFortyFourDot());}
        inline double& CrFortyEightDot() { IMAGAS; return (((extraSPHData*)extraData)->CrFortyEightDot());}
        inline double& FeFiftyTwoDot() { IMAGAS; return (((extraSPHData*)extraData)->FeFiftyTwoDot());}
        inline double& NiFiftySixDot() { IMAGAS; return (((extraSPHData*)extraData)->NiFiftySixDot());}
	inline double& HeFourPred() { IMAGAS; return (((extraSPHData*)extraData)->HeFourPred());}
	inline double& CTwelvePred() { IMAGAS; return (((extraSPHData*)extraData)->CTwelvePred());}
	inline double& OSixteenPred() { IMAGAS; return (((extraSPHData*)extraData)->OSixteenPred());}
        inline double& NeTwentyPred() { IMAGAS; return (((extraSPHData*)extraData)->NeTwentyPred());}
        inline double& MgTwentyFourPred() { IMAGAS; return (((extraSPHData*)extraData)->MgTwentyFourPred());}
        inline double& SiTwentyEightPred() { IMAGAS; return (((extraSPHData*)extraData)->SiTwentyEightPred());}
        inline double& SThirtyTwoPred()  { IMAGAS; return (((extraSPHData*)extraData)->SThirtyTwoPred());}
        inline double& ArThirtySixPred() { IMAGAS; return (((extraSPHData*)extraData)->ArThirtySixPred());}
        inline double& CaFortyPred() { IMAGAS; return (((extraSPHData*)extraData)->CaFortyPred());}
        inline double& TiFortyFourPred() { IMAGAS; return (((extraSPHData*)extraData)->TiFortyFourPred());}
        inline double& CrFortyEightPred() { IMAGAS; return (((extraSPHData*)extraData)->CrFortyEightPred());}
        inline double& FeFiftyTwoPred() { IMAGAS; return (((extraSPHData*)extraData)->FeFiftyTwoPred());}
        inline double& NiFiftySixPred() { IMAGAS; return (((extraSPHData*)extraData)->NiFiftySixPred());}

#endif
	inline double& PoverRho2() { IMAGAS; return (((extraSPHData*)extraData)->PoverRho2());}
	inline double& BalsaraSwitch() { IMAGAS; return (((extraSPHData*)extraData)->BalsaraSwitch());}
	inline double& fBallMax() { IMAGAS; return (((extraSPHData*)extraData)->fBallMax());}
#ifndef COOLING_NONE
	inline double& uDot() { IMAGAS; return (((extraSPHData*)extraData)->uDot());}
	inline COOLPARTICLE& CoolParticle() { IMAGAS; return (((extraSPHData*)extraData)->CoolParticle());}
#endif
	// Access Star Quantities
	// XXX Beware overlaps with SPH; we could fix this by aligning
	// all common variables up at the start of the extraData structure.
	inline double& fStarMetals() { IMASTAR; return (((extraStarData*)extraData)->fMetals());}
	inline double& fStarMFracOxygen() {IMASTAR; return (((extraStarData*)extraData)->fMFracOxygen());}
	inline double& fStarMFracIron() {IMASTAR; return (((extraStarData*)extraData)->fMFracIron());}
	inline double& fTimeForm() { IMASTAR; return (((extraStarData*)extraData)->fTimeForm());}
	inline double& fMassForm() { IMASTAR; return (((extraStarData*)extraData)->fMassForm());}
	inline double& fStarESNrate() {IMASTAR; return (((extraStarData*)extraData)->fESNrate());}
	inline double& fNSN() {IMASTAR; return (((extraStarData*)extraData)->fNSN());}
	inline double& fMSN() {IMASTAR; return (((extraStarData*)extraData)->fMSN());}
	inline double& fMIronOut() {IMASTAR; return (((extraStarData*)extraData)->fMIronOut());}
	inline double& fMOxygenOut() {IMASTAR; return (((extraStarData*)extraData)->fMOxygenOut());}
	inline double& fSNMetals() {IMASTAR; return (((extraStarData*)extraData)->fSNMetals());}
	inline int64_t& iGasOrder() { IMASTAR; return (((extraStarData*)extraData)->iGasOrder());}

// See above debugging macros
#undef IMAGAS
#undef IMASTAR

/* Particle Type Masks */

#define TYPE_GAS               (1<<0)
#define TYPE_DARK              (1<<1)
#define TYPE_STAR              (1<<2)

#define TYPE_DELETED           (1<<3)

#define TYPE_PHOTOGENIC        (1<<4)
#define TYPE_NbrOfACTIVE       (1<<5)

	inline bool isDark() const { return TYPETest(this, TYPE_DARK);}
	inline bool isGas() const { return TYPETest(this, TYPE_GAS);}
	inline bool isStar() const { return TYPETest(this, TYPE_STAR);}

        GravityParticle &operator=(const ExternalGravityParticle &p){
          mass = p.mass;
          soft = p.soft;
          position = p.position;
	  return *this;
        }
};

inline int TYPETest(const GravityParticle *a, unsigned int b) {
    return a->iType & b;
    }
inline int TYPESet(GravityParticle *a, unsigned int b) {
    return a->iType |= b;
    }
inline int TYPEReset(GravityParticle *a, unsigned int b) {
    return a->iType &= (~b);
    }

/// @brief unmark particle as deleted
inline void unDeleteParticle(GravityParticle *p)
{
    CkAssert(TYPETest(p, TYPE_DELETED)); 

    TYPEReset(p, TYPE_DELETED); 
    }

/// @brief mark particle as deleted
inline void deleteParticle(GravityParticle *p)
{
    TYPESet(p, TYPE_DELETED); 
    }

/// @brief Create star particle from gas particle
/// Note that new memory is allocated for the extradata.
inline GravityParticle StarFromGasParticle(GravityParticle *p) 
{
    GravityParticle starp = *p;
    TYPESet(&starp, TYPE_STAR);
    starp.extraData = new extraStarData;
    starp.fStarMetals() = p->fMetals();
    starp.fStarMFracOxygen() = p->fMFracOxygen();
    starp.fStarMFracIron() = p->fMFracIron();
    return starp;
    }

/// @brief Class for cross processor data needed for smooth operations
class ExternalSmoothParticle {
 public:

  double mass;
  double fBall;
  double fDensity;
  Vector3D<double> position;
  Vector3D<double> velocity;
  unsigned int iType;	// Bitmask to hold particle type information
  int rung;
  Vector3D<double> vPred;
  Vector3D<double> treeAcceleration;
  double mumax;
  double PdV;
  double c;
#ifdef COOLING_NUC
  double HeFour;
  double CTwelve;
  double OSixteen;
  double NeTwenty;
  double MgTwentyFour;
  double SiTwentyEight;
  double SThirtyTwo;
  double ArThirtySix;
  double CaForty;
  double TiFortyFour;
  double CrFortyEight;
  double FeFiftyTwo;
  double NiFiftySix;
  double HeFourDot;
  double CTwelveDot;
  double OSixteenDot;
  double NeTwentyDot;
  double MgTwentyFourDot;
  double SiTwentyEightDot;
  double SThirtyTwoDot;
  double ArThirtySixDot;
  double CaFortyDot;
  double TiFortyFourDot;
  double CrFortyEightDot;
  double FeFiftyTwoDot;
  double NiFiftySixDot;
  double HeFourPred;
  double CTwelvePred;
  double OSixteenPred;
  double NeTwentyPred;
  double MgTwentyFourPred;
  double SiTwentyEightPred;
  double SThirtyTwoPred;
  double ArThirtySixPred;
  double CaFortyPred;
  double TiFortyFourPred;
  double CrFortyEightPred;
  double FeFiftyTwoPred;
  double NiFiftySixPred;
#endif
  double PoverRho2;
  double BalsaraSwitch;
  double fBallMax;
  double u;
  double uPred;
  double uDot;
  double fESNrate;
  double fMetals;
  double fMFracOxygen;
  double fMFracIron;
  double fTimeCoolIsOffUntil;
  Vector3D<double> curlv;	/* Curl of the velocity */
  int iBucketOff;               /* Used by the Cache */

  ExternalSmoothParticle() {}

  ExternalSmoothParticle(GravityParticle *p) 
      {
	  mass = p->mass;
	  fBall = p->fBall;
	  fDensity = p->fDensity;
	  position = p->position;
	  velocity = p->velocity;
	  iType = p->iType;
	  rung = p->rung;
	  treeAcceleration = p->treeAcceleration;
	  if(TYPETest(p, TYPE_GAS)) {
	      vPred = p->vPred();
	      mumax = p->mumax();
	      PdV = p->PdV();
	      c = p->c();
#ifdef COOLING_NUC
	      HeFour = p->HeFour();
	      CTwelve = p->CTwelve();
	      OSixteen = p->OSixteen();
	      NeTwenty = p->NeTwenty();
              MgTwentyFour = p->MgTwentyFour();
	      SiTwentyEight = p->SiTwentyEight();
	      SThirtyTwo = p->SThirtyTwo();
	      ArThirtySix = p->ArThirtySix();
 	      CaForty = p->CaForty();
	      TiFortyFour = p->TiFortyFour();
              CrFortyEight = p->CrFortyEight();
              FeFiftyTwo = p->FeFiftyTwo();
              NiFiftySix = p->NiFiftySix();
	      HeFourDot = p->HeFourDot();
	      CTwelveDot = p->CTwelveDot();
	      OSixteenDot = p->OSixteenDot();
	      NeTwentyDot = p->NeTwentyDot();
              MgTwentyFourDot = p->MgTwentyFourDot();
	      SiTwentyEightDot = p->SiTwentyEightDot();
	      SThirtyTwoDot = p->SThirtyTwoDot();
	      ArThirtySixDot = p->ArThirtySixDot();
 	      CaFortyDot = p->CaFortyDot();
	      TiFortyFourDot = p->TiFortyFourDot();
              CrFortyEightDot = p->CrFortyEightDot();
              FeFiftyTwoDot = p->FeFiftyTwoDot();
              NiFiftySixDot = p->NiFiftySixDot();
	      HeFourPred = p->HeFourPred();
	      CTwelvePred = p->CTwelvePred();
	      OSixteenPred = p->OSixteenPred();
	      NeTwentyPred = p->NeTwentyPred();
              MgTwentyFourPred = p->MgTwentyFourPred();
	      SiTwentyEightPred = p->SiTwentyEightPred();
	      SThirtyTwoPred = p->SThirtyTwoPred();
	      ArThirtySixPred = p->ArThirtySixPred();
 	      CaFortyPred = p->CaFortyPred();
	      TiFortyFourPred = p->TiFortyFourPred();
              CrFortyEightPred = p->CrFortyEightPred();
              FeFiftyTwoPred = p->FeFiftyTwoPred();
              NiFiftySixPred = p->NiFiftySixPred();

#endif
	      PoverRho2 = p->PoverRho2();
	      BalsaraSwitch = p->BalsaraSwitch();
	      fBallMax = p->fBallMax();
	      curlv = p->curlv();
	      u = p->u();
#ifndef COOLING_NONE
	      uDot = p->uDot();
	      uPred = p->uPred();
#endif
	      fMetals = p->fMetals();
	      fESNrate = p->fESNrate();
	      fMFracOxygen = p->fMFracOxygen();
	      fMFracIron = p->fMFracIron();
	      fTimeCoolIsOffUntil = p->fTimeCoolIsOffUntil();
	      }
	  }
  
  /// @brief Fill in a full gravity particle from this object.
  inline void getParticle(GravityParticle *tmp) const { 
      tmp->mass = mass;
      tmp->fBall = fBall;
      tmp->fDensity = fDensity;
      tmp->position = position;
      tmp->velocity = velocity;
      tmp->iType = iType;
      tmp->rung = rung;
      tmp->treeAcceleration = treeAcceleration;
      if(TYPETest(tmp, TYPE_GAS)) {
	  tmp->vPred() = vPred;
	  tmp->mumax() = mumax;
	  tmp->PdV() = PdV;
	  tmp->c() = c;
#ifdef COOLING_NUC
	  tmp->HeFour() = HeFour;
	  tmp->CTwelve() = CTwelve;
	  tmp->OSixteen() = OSixteen;
	  tmp->NeTwenty() = NeTwenty;
	  tmp->MgTwentyFour() = MgTwentyFour;
	  tmp->SiTwentyEight() = SiTwentyEight;
	  tmp->SThirtyTwo() = SThirtyTwo;
	  tmp->ArThirtySix() = ArThirtySix;
	  tmp->CaForty() = CaForty;
	  tmp->TiFortyFour() = TiFortyFour;
 	  tmp->CrFortyEight() = CrFortyEight;
	  tmp->FeFiftyTwo() = FeFiftyTwo;
	  tmp->NiFiftySix() = NiFiftySix;
	  tmp->HeFourDot() = HeFourDot;
	  tmp->CTwelveDot() = CTwelveDot;
	  tmp->OSixteenDot() = OSixteenDot;
	  tmp->NeTwentyDot() = NeTwentyDot;
	  tmp->MgTwentyFourDot() = MgTwentyFourDot;
	  tmp->SiTwentyEightDot() = SiTwentyEightDot;
	  tmp->SThirtyTwoDot() = SThirtyTwoDot;
	  tmp->ArThirtySixDot() = ArThirtySixDot;
	  tmp->CaFortyDot() = CaFortyDot;
	  tmp->TiFortyFourDot() = TiFortyFourDot;
 	  tmp->CrFortyEightDot() = CrFortyEightDot;
	  tmp->FeFiftyTwoDot() = FeFiftyTwoDot;
	  tmp->NiFiftySixDot() = NiFiftySixDot; 
	  tmp->HeFourPred() = HeFourPred;
	  tmp->CTwelvePred() = CTwelvePred;
	  tmp->OSixteenPred() = OSixteenPred;
	  tmp->NeTwentyPred() = NeTwentyPred;
	  tmp->MgTwentyFourPred() = MgTwentyFourPred;
	  tmp->SiTwentyEightPred() = SiTwentyEightPred;
	  tmp->SThirtyTwoPred() = SThirtyTwoPred;
	  tmp->ArThirtySixPred() = ArThirtySixPred;
	  tmp->CaFortyPred() = CaFortyPred;
	  tmp->TiFortyFourPred() = TiFortyFourPred;
 	  tmp->CrFortyEightPred() = CrFortyEightPred;
	  tmp->FeFiftyTwoPred() = FeFiftyTwoPred;
	  tmp->NiFiftySixPred() = NiFiftySixPred; 

#endif
	  tmp->PoverRho2() = PoverRho2;
	  tmp->BalsaraSwitch() = BalsaraSwitch;
	  tmp->fBallMax() = fBallMax;
	  tmp->curlv() = curlv;
	  tmp->u() = u;
#ifndef COOLING_NONE
	  tmp->uDot() = uDot;
	  tmp->uPred() = uPred;
#endif
	  tmp->fMetals() = fMetals;
	  tmp->fESNrate() = fESNrate;
	  tmp->fMFracOxygen() = fMFracOxygen;
	  tmp->fMFracIron() = fMFracIron;
	  tmp->fTimeCoolIsOffUntil() = fTimeCoolIsOffUntil;
	  }
      }
	  
  void pup(PUP::er &p) {
    p | position;
    p | velocity;
    p | mass;
    p | fBall;
    p | fDensity;
    p | iType;
    p | rung;
    p | treeAcceleration;
    p | vPred;
    p | mumax;
    p | PdV;
    p | c;
#ifdef COOLING_NUC
    p | HeFour;
    p | CTwelve;
    p | OSixteen;
    p | NeTwenty;
    p | MgTwentyFour;
    p | SiTwentyEight;
    p | SThirtyTwo;
    p | ArThirtySix;
    p | CaForty;
    p | TiFortyFour;
    p | CrFortyEight;
    p | FeFiftyTwo;
    p | NiFiftySix;
    p | HeFourDot;
    p | CTwelveDot;
    p | OSixteenDot;
    p | NeTwentyDot;
    p | MgTwentyFourDot;
    p | SiTwentyEightDot;
    p | SThirtyTwoDot;
    p | ArThirtySixDot;
    p | CaFortyDot;
    p | TiFortyFourDot;
    p | CrFortyEightDot;
    p | FeFiftyTwoDot;
    p | NiFiftySixDot;
    p | HeFourPred;
    p | CTwelvePred;
    p | OSixteenPred;
    p | NeTwentyPred;
    p | MgTwentyFourPred;
    p | SiTwentyEightPred;
    p | SThirtyTwoPred;
    p | ArThirtySixPred;
    p | CaFortyPred;
    p | TiFortyFourPred;
    p | CrFortyEightPred;
    p | FeFiftyTwoPred;
    p | NiFiftySixPred;

#endif
    p | PoverRho2;
    p | BalsaraSwitch;
    p | fBallMax;
    p | u;
    p | uPred;
    p | uDot;
    p | curlv;
    p | fMetals;
    p | fESNrate;
    p | fMFracOxygen;
    p | fMFracIron;
    p | fTimeCoolIsOffUntil;
    p | iBucketOff;
  }
};

inline ExternalSmoothParticle GravityParticle::getExternalSmoothParticle()
{ return ExternalSmoothParticle(this); }

#endif
