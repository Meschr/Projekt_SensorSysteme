#ifndef _IINCREMENTALENCODER_hf
#define _IINCREMENTALENCODER_hf

#include "IPositionMeasurement.h"

// Interface der Wegmessung mit Inkrementalgeber
class IIncrementalEncoder : public IPositionMeasurement
{
public:
    IIncrementalEncoder(unsigned int in_incPerMm) : IPositionMeasurement(), mIncPerMm(in_incPerMm) {}
    ~IIncrementalEncoder(void) {}

    // Initialisierung
    virtual void Init(void) = 0;
    
    // Aktueller Weg in mm
    virtual double GetPositionMm(void) { return (mIncPerMm > 0 ? ReadCnt() / mIncPerMm : 0); }

protected:
    // Auslesen der gezaehlten Inkremente
    virtual unsigned int ReadCnt(void);

    unsigned int mIncPerMm; // Inkremente pro mm
};

#endif // #ifndef _IINCREMENTALENCODER_hf