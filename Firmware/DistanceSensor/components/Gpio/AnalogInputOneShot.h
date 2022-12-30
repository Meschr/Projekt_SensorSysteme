#ifndef _IANALOGINPUTONESHOT_hf
#define _IANALOGINPUTONESHOT_hf

#include "IAnalogInput.h"

static const char* TAG_OS = "AIN_OS";


class AnalogInputOneShot: public IAnalogInput
{
private:
    adc_oneshot_unit_handle_t mAdcHandle;
    adc_oneshot_unit_init_cfg_t mInitConfig;
public:
    AnalogInputOneShot(adc_unit_t unit, adc_channel_t channel, adc_atten_t attenuation);
    ~AnalogInputOneShot();

    int GetGpioNum();
    int GetRawValue();
    int GetRawValueWithErrorCheck();
    double GetRawValueAndConvertToVoltage();
    double GetVoltageCalibrated();
};

AnalogInputOneShot::AnalogInputOneShot(adc_unit_t unit, adc_channel_t channel, adc_atten_t attenuation) : IAnalogInput(unit, channel, attenuation)
{
    mInitConfig = {
    .unit_id = mAdcUnit,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&mInitConfig, &mAdcHandle));

    adc_oneshot_chan_cfg_t config = {
    .atten = mAttenuation,
    .bitwidth = ADC_BITWIDTH_12,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(mAdcHandle, mAdcChannel, &config));
}

AnalogInputOneShot::~AnalogInputOneShot()
{
    ESP_ERROR_CHECK(adc_oneshot_del_unit(mAdcHandle));
}

int AnalogInputOneShot::GetGpioNum()
{   
    int gpioNum[1];
    ESP_ERROR_CHECK(adc_oneshot_channel_to_io(mAdcUnit, mAdcChannel, gpioNum));

    ESP_LOGI(TAG_OS, "Gpio_Num: %d", gpioNum[0]);

    return *gpioNum;
}

int AnalogInputOneShot::GetRawValue()
{
    int adc_raw[1];
    adc_oneshot_read(mAdcHandle, mAdcChannel, adc_raw);
    ESP_LOGI(TAG_OS, "Raw_Adc_Reading: %d", adc_raw[0]);
    return adc_raw[0];
}

int AnalogInputOneShot::GetRawValueWithErrorCheck()
{
    int* adc_raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(mAdcHandle, mAdcChannel, adc_raw));
    
    return adc_raw[0];
}

double AnalogInputOneShot::GetRawValueAndConvertToVoltage()
{   
    //TODO
    double Vout = 0.0;    //Digital output result, standing for the voltage.
    int Dout[1];          //ADC raw digital reading result.
    double Vmax = 2450.0; //Maximum measurable input analog voltage, this is related to the ADC attenuation
    uint Dmax = 4096;     //Maximum of the output ADC raw digital reading result, which is 2^bitwidth
    
    adc_oneshot_read(mAdcHandle, mAdcChannel, Dout);
    Vout = static_cast<double>(Dout[0]) * (Vmax / Dmax);

    ESP_LOGI(TAG_OS, "Converted_Adc_Reading: %f in mV", Vout);

    return Vout;
}

double AnalogInputOneShot::GetVoltageCalibrated()
{
    int rawValue[1];
    int voltage[1];
    adc_oneshot_read(mAdcHandle, mAdcChannel, rawValue);
    adc_cali_raw_to_voltage(mAdcCalibrationHandle, rawValue[0], &voltage[0]);

    //ESP_LOGI(TAG_OS, "%d", voltage[0]);

    return voltage[0];
}

#endif // #ifndef _IANALOGINPUTONESHOT_hf