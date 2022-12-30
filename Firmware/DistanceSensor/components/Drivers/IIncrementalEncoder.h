#ifndef _IINCREMENTALENCODER_hf
#define _IINCREMENTALENCODER_hf

#include "IPositionMeasurement.h"

// Interface der Wegmessung mit Inkrementalgeber
class IIncrementalEncoder : public IPositionMeasurement
{
public:
    IIncrementalEncoder(unsigned int IncPerRev, unsigned int QuadratureMode) : IPositionMeasurement(), mIncPerRev(IncPerRev), mQuadratureMode(QuadratureMode) 
    {
        mIncPerMm = mIncPerRev*mQuadratureMode;
    }
    ~IIncrementalEncoder(void) {}

    // Initialisierung
    virtual void Init(void) = 0;
    
    // Aktueller Weg in mm
    virtual double GetPositionMm(void) { return (double)ReadCnt() * (40.0 / (double)mIncPerMm); } //TODO refactor this (40mm RollenUmfang)

    virtual void ResetPosition(void){ ResetCnt(); }  

protected:
    // Auslesen der gezaehlten Inkremente
    virtual int ReadCnt(void) = 0;

    virtual void ResetCnt(void) = 0;

    unsigned int mIncPerRev;      //Inkremente pro Umdrehung
    unsigned int mQuadratureMode; //Quadraturmodus (1-4fach)
    unsigned int mIncPerMm;       //Inkremente pro 
};

#endif // #ifndef _IINCREMENTALENCODER_hf