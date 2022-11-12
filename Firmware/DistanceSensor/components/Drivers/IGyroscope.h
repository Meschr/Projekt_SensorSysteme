#ifndef _IGYROSCOPE_hf
#define _IGYROSCOPE_hf

#include <LogData.h>

class IGyroscope
{
public:
    IGyroscope(void) {}
    virtual ~IGyroscope(void) {}

    // Initialisierung
    virtual void Init(void) = 0;
    
    // Aktuelle Rotation
    virtual SRawRotationData GetRotation(void) = 0;

    //TODO implement convert to rotation function and add to mpu6050
};

#endif //#ifndef _IGYROSCOPE_hf