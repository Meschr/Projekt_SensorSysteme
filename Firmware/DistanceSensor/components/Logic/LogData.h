#ifndef _LogData_hf
#define _LogData_hf

struct SAccelerationData
{
    float acceleration_x = 0.0;
    float acceleration_y = 0.0;
    float acceleration_z = 0.0;
};

struct SRotationData
{
    float rotation_x = 0.0;
    float rotation_y = 0.0;
    float rotation_z = 0.0;
};

struct SRawAccelerationData
{
    int16_t acceleration_x = 0;
    int16_t acceleration_y = 0;
    int16_t acceleration_z = 0;
};

struct SRawRotationData
{
    int16_t rotation_x = 0;
    int16_t rotation_y = 0;
    int16_t rotation_z = 0;
};

struct SLogData
{
    bool marker;
    double pos = 0;
    double adc0 = 0;
    unsigned int index = 0;

    SAccelerationData acceleration_data;
};

#endif // #ifndef _LogData_hf