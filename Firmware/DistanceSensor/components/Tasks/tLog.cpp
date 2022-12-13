#include "tLog.h"

#include <atomic>
#include <fstream>
#include <sys/stat.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "tasks.h"
#include "DataLogStateMachine.h"
#include "GpioInit.h"

static const char* TAG = "tLog";

void tLog(void* pvParameters)
{
    ESP_LOGI(TAG, "Task started");
    
    CDataLogStateMachine::CreateInstance();
    CDataLogStateMachine::GetInstance()->Init();
    InitGpio();

    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 100/portTICK_PERIOD_MS; // 100ms

    flagrestart-=restart_log;
    while (flagrestart != 0) {}
    for(;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        
        // LED-Status setzen
        if(CDataLogStateMachine::GetInstance()->IsLogging())
        {
            EnableDisableLoggingLED(1);
        }
        else
        {
            EnableDisableLoggingLED(0);
        }

        CDataLogStateMachine::GetInstance()->Receive();
    }
}