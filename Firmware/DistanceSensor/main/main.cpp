#include "tFast.h"
#include "tLog.h"
#include "tasks.h"

#include <atomic>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mpu6050.h"
#include "esp_log.h"

static const char* TAG = "marvin stinkt";

std::atomic<int> flagrestart = ATOMIC_VAR_INIT(restart_fast|restart_log);
TaskHandle_t Task1, Task2 = NULL;

extern "C" int app_main(void)
{
    //xTaskCreatePinnedToCore(tFast, "tFast", 4098*8, NULL, 1, &Task1, 1);
    //xTaskCreatePinnedToCore(tLog, "tLog", 4098*8, NULL, 2, &Task2, 0);

    CMpu6050* mCMpu6050 = new CMpu6050(MPU6050_ADDRESS_LOW, ACCEL_FULL_SCALE_RANGE_2, GYRO_FULL_SCALE_RANGE_250);
    mCMpu6050->Init();
    auto init_success = mCMpu6050->TestConnection() ? "mpu6050 initialized successfully!" : "mpu6050 test connection failed!";
    ESP_LOGI(TAG, "%s",init_success);

    CMpu6050* mCMpu6050_kabel = new CMpu6050(MPU6050_ADDRESS_HIGH, ACCEL_FULL_SCALE_RANGE_2, GYRO_FULL_SCALE_RANGE_250);
    mCMpu6050_kabel->Init();

    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 500 / portTICK_PERIOD_MS; // 1s 
    for( ;; )
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
           
        // Perform action here.
        mCMpu6050->GetAndConvertAccelerationX();
        mCMpu6050_kabel->GetAndConvertAccelerationX();
    }




    //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/startup.html#:~:text=also%20configurable%3A%20CONFIG_ESP_MAIN_TASK_AFFINITY.-,Unlike,-normal%20FreeRTOS%20tasks
    return EXIT_SUCCESS; 
}