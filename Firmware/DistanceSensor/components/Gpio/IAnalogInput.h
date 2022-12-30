#ifndef _IANALOGINPUT_hf
#define _IANALOGINPUT_hf

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char* TAG_AIN = "AIN";

//ADC_ATTEN_DB_0	100 ~ 950	No attenuation (0dB)
//ADC_ATTEN_DB_2_5	100 ~ 1250	2.5 dB attenuation
//ADC_ATTEN_DB_6	150 ~ 1750	6 dB attenuation
//ADC_ATTEN_DB_11	150 ~ 2450	11 dB attenuation

class IAnalogInput 
{
  public:

    IAnalogInput(adc_unit_t unit, adc_channel_t channel, adc_atten_t attenuation) : mAdcUnit(unit), mAdcChannel(channel), mAttenuation(attenuation) {}

    // Calibrate ADC
    void Calibrate() 
    {
      if(true)
      {
        adc_cali_line_fitting_config_t adcCaliLineFittingConfig {
          .unit_id = mAdcUnit,
          .atten = mAttenuation,
          .bitwidth = ADC_BITWIDTH_12,
  
        };

        ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&adcCaliLineFittingConfig, &mAdcCalibrationHandle));

      }
      else
      {
        ESP_LOGE(TAG_AIN,"Cannot Calibrate because Efuse bit is burned !");
      }
    }

    //Get the GPIO Num from the ADC unit and channel
    virtual int GetGpioNum() = 0;

    // Read raw analog input value
    virtual int GetRawValue() = 0;

    // Read raw analog input value with an Error Check
    virtual int GetRawValueWithErrorCheck() = 0;

    // Read raw analog input value an convert to Voltage
    virtual double GetRawValueAndConvertToVoltage() = 0;

    // Read raw analog input value an get Voltage calibrated
    virtual double GetVoltageCalibrated() = 0;

  protected:
    adc_unit_t mAdcUnit;
    adc_channel_t mAdcChannel;
    adc_atten_t mAttenuation;
    uint mAdcInternalVref = 1135; //saved in esp-eFuse
    adc_cali_handle_t mAdcCalibrationHandle = NULL;
    
};

#endif // #ifndef _IANALOGINPUT_hf