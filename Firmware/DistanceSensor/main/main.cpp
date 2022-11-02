#include "tFast.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

TaskHandle_t Task1, Task2 = NULL;

extern "C" void app_main(void)
{
    xTaskCreatePinnedToCore(tFast, "tFast", 4098*8, NULL, 1, &Task1, 1);

    //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/startup.html#:~:text=also%20configurable%3A%20CONFIG_ESP_MAIN_TASK_AFFINITY.-,Unlike,-normal%20FreeRTOS%20tasks
    return; 
}