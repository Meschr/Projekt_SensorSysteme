#include "GpioInit.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "DataLogStateMachine.h"

static const gpio_num_t GPIO_START_LOGGING = GPIO_NUM_16;   // Button start logging
static const gpio_num_t GPIO_SET_MARKER = GPIO_NUM_17;      // Button set marker
static const gpio_num_t GPIO_LED_LOGGING = GPIO_NUM_33;     // LED indicate logging started
#define GPIO_LED 33
static const int ESP_INTR_FLAG_DEFAULT = 0;

void IRAM_ATTR button_start_log_isr_handler(void *arg)
{
    if(CDataLogStateMachine::GetInstance()->IsLogging())
    {
        CDataLogStateMachine::GetInstance()->StopLogging();
    }
    else
    {
        CDataLogStateMachine::GetInstance()->StartLogging();
    }
}

void IRAM_ATTR button_set_marker_isr_handler(void *arg)
{
    CDataLogStateMachine::GetInstance()->SetMarker();
}

void InitGpio()
{
    gpio_pad_select_gpio(GPIO_START_LOGGING);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_INPUT);
    gpio_set_intr_type(GPIO_NUM_16, GPIO_INTR_POSEDGE);
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_INPUT);
    gpio_set_intr_type(GPIO_NUM_17, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_NUM_16, button_start_log_isr_handler, NULL);
    gpio_isr_handler_add(GPIO_NUM_17, button_set_marker_isr_handler, NULL);
    gpio_set_direction(GPIO_LED_LOGGING, GPIO_MODE_OUTPUT); 

    ledc_timer_config_t timer_config;
    timer_config.speed_mode = LEDC_LOW_SPEED_MODE; 
    timer_config.duty_resolution = (ledc_timer_bit_t)16;//LEDC_TIMER_15_BIT;
    timer_config.timer_num = LEDC_TIMER_0;
    timer_config.freq_hz = 2;
    timer_config.clk_cfg = LEDC_USE_APB_CLK;
    ledc_timer_config(&timer_config);

    ledc_channel_config_t ledc_config;
    ledc_config.gpio_num = GPIO_LED;
    ledc_config.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_config.channel = LEDC_CHANNEL_0;
    ledc_config.intr_type = LEDC_INTR_DISABLE;
    ledc_config.timer_sel = LEDC_TIMER_0;
    ledc_config.duty = 0x0;
    ledc_config.hpoint = 0;
    ledc_channel_config(&ledc_config);
}

void EnableDisableLoggingLED(bool state)
{
    if(state)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 16000);
        ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
    }
}