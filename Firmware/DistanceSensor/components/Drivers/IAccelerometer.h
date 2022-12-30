#ifndef _IACCELEROMETER_hf
#define _IACCELEROMETER_hf

#include <LogData.h>

class IAccelerometer
{
public:
    IAccelerometer(void) {}
    virtual ~IAccelerometer(void) {}

    // Initialisierung
    virtual void Init(void) = 0;

    // Aktuelle Beschleunigung raw daten
    virtual SRawAccelerationData GetAcceleration(void) = 0;

    // Aktuelle Beschleunigung umgerechnete daten
    virtual SAccelerationData GetAndConvertAcceleration() = 0;

    //Aktuelle Beschleunigung in X-Richtung
    virtual double GetAndConvertAccelerationX() = 0;
};

#endif //#ifndef _IACCELEROMETER_hf