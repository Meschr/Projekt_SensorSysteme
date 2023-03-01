#ifndef _IMU_hf
#define _IMU_hf

#include "IAccelerometer.h"
#include "IGyroscope.h"
#include "ITemperatureSensor.h"

class IInertialMeasurementUnit : public IAccelerometer, public IGyroscope, public ITemperatureSensor
{
public:
    IInertialMeasurementUnit(void) {}
    virtual ~IInertialMeasurementUnit(void) {}

    // Initialisierung
    virtual void Init(void) = 0;

    // Kalibrierung
    virtual void Calibrate(float *accel_bias_res, float *gyro_bias_res) = 0; 

    virtual double GetAndConvertAccelerationY() = 0;
};

#endif // #ifndef _IMU_hf