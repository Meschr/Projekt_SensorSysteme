#include "tFast.h"

#include <atomic>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "tasks.h"
#include "DataLogStateMachine.h"

static const char* TAG = "tFast";

void tFast(void* pvParameters)
{
    ESP_LOGI(TAG, "Task started");
    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 1000 / portTICK_PERIOD_MS; // 1000ms 

    flagrestart-=restart_fast;
    while (flagrestart != 0) {}
    for( ;; )
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
           
        // Perform action here.
        CDataLogStateMachine::GetInstance()->Send();
    }
}