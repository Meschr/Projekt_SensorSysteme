#ifndef _ITEMPSENSOR_hf
#define _ITEMPSENSOR_hf

class ITemperatureSensor
{
public:
    ITemperatureSensor(void) {}
    virtual ~ITemperatureSensor(void) {}

    // Initialisierung
    virtual void Init(void) = 0;

    // Aktuelle Temperatur raw 
    virtual int16_t GetTemperature(void) = 0;

    // Aktuelle Temperatur umgerechnet in Grad Celsius
    float GetAndConvertTemperatureToCelsius();
};

#endif // #ifndef _ITEMPSENSOR_hf