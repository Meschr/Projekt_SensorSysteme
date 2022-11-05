#ifndef _IACCELEROMETER_hf
#define _IACCELEROMETER_hf

#include <LogData.h>

// Interface der Beschleunigungsmessung
class IAccelerometer
{
public:
    IAccelerometer(void) {}
    virtual ~IAccelerometer(void) {}

    // Initialisierung
    virtual void Init(void) = 0;

    // Aktuelle Beschleunigung
    virtual SAccelerationData GetAcceleration(void) = 0;
    
    // Aktuelle Rotation
    virtual SRotationData GetRotation(void) = 0;
};

#endif // #ifndef _IACCELEROMETER_hf