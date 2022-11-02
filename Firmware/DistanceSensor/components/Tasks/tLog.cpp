#include "tLog.h"

#include <atomic>
#include <fstream>
#include <sys/stat.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "DataLogStateMachine.h"
#include "GpioInit.h"

#include "tasks.h"

void tLog(void* pvParameters)
{
    CDataLogStateMachine::CreateInstance();
    CDataLogStateMachine::GetInstance()->Init();
    InitGpio();

    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500ms

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
