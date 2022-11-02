#ifndef _LogData_hf
#define _LogData_hf

struct SAccelerationData
{
    int acceleration_x = 0;
    int acceleration_y = 0;
    int acceleration_z = 0;
};

struct SRotationData
{
    int rotation_x = 0;
    int rotation_y = 0;
    int rotation_z = 0;
};

struct SLogData
{
    bool marker;
    double pos = 0;
    int adc0 = 0;
    unsigned int index = 0;

    SAccelerationData acceleration_data;
};

#endif // #ifndef _LogData_hf