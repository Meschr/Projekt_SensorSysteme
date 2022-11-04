#include "tFast.h"
#include "tLog.h"
#include "tasks.h"

#include <atomic>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mpu6050.h"
static const char* TAG = "MAIN:";

std::atomic<int> flagrestart = ATOMIC_VAR_INIT(restart_fast|restart_log);
TaskHandle_t Task1, Task2 = NULL;

extern "C" void app_main(void)
{
    mpu6050_init();
    auto init_success = mpu6050_test_connection() ? "mpu6050 initialized successfully!" : "mpu6050 test connection failed!";
    ESP_LOGI(TAG, "%s",init_success);

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 100 / portTICK_PERIOD_MS; // 1s 
    for( ;; )
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
           
        // Perform action here.
        auto accel_x = mpu6050_get_acceleration_x();
        printf("Accel_x: %d\n", accel_x);
    }


    //xTaskCreatePinnedToCore(tFast, "tFast", 4098*8, NULL, 1, &Task1, 1);
    //xTaskCreatePinnedToCore(tLog, "tLog", 4098*8, NULL, 2, &Task2, 0);

    //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/startup.html#:~:text=also%20configurable%3A%20CONFIG_ESP_MAIN_TASK_AFFINITY.-,Unlike,-normal%20FreeRTOS%20tasks
    return; 
}