#include "tFast.h"

#include <atomic>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "tasks.h"
#include "DataLogStateMachine.h"
#include "LogInfoHandler.h"

static const char* TAG = "tFast";

int ConvertSamplingFrequencyToSamplingTimeInMs(int frequency)
{
    return (int) 1000/frequency;
}


void tFast(void* pvParameters)
{
    ESP_LOGI(TAG, "Task started");
    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = ConvertSamplingFrequencyToSamplingTimeInMs(SamplingFrequency) / portTICK_PERIOD_MS; 
    
    CLogInfoHandler::GetInstance()->mLogInfo.SamplingFreq = SamplingFrequency;

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