#include "tFast.h"
#include "tLog.h"
#include "tasks.h"

#include <atomic>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "IAnalogInput.h"
#include "AnalogInputOneShot.h"

std::atomic<int> flagrestart = ATOMIC_VAR_INIT(restart_fast|restart_log);
TaskHandle_t Task1, Task2 = NULL;

extern "C" int app_main(void)
{   
    auto analogInput = new AnalogInputOneShot(ADC_UNIT_1, ADC_CHANNEL_0, ADC_ATTEN_DB_11);
    analogInput->GetGpioNum();

    for(;;)
    {
        analogInput->GetRawValue();
        analogInput->GetRawValueAndConvertToVoltage();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    xTaskCreatePinnedToCore(tFast, "tFast", 4098*8, NULL, 1, &Task1, 1);
    xTaskCreatePinnedToCore(tLog, "tLog", 4098*8, NULL, 2, &Task2, 0);

    //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/startup.html#:~:text=also%20configurable%3A%20CONFIG_ESP_MAIN_TASK_AFFINITY.-,Unlike,-normal%20FreeRTOS%20tasks
    return EXIT_SUCCESS; 
}