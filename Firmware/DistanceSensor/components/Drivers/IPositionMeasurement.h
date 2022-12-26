#ifndef _IPOSITIONMEASUREMENT_hf
#define _IPOSITIONMEASUREMENT_hf

// Interface der Wegmessung
class IPositionMeasurement
{
public:
    IPositionMeasurement(void) {}
    virtual ~IPositionMeasurement(void) {}

    // Initialisierung
    virtual void Init(void) = 0;

    // Aktueller Weg in mm
    virtual double GetPositionMm(void) = 0;

    virtual void ResetPosition(void) = 0;
};

#endif // #ifndef _IPOSITIONMEASUREMENT_hf