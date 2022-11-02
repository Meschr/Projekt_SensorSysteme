#include "DataLogStateMachine.h"
#include <sys/stat.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

//#include "Measurement/Acceleration/MPU6050Accelerometer.h"
#include "Sdmmc.h"

CDataLogStateMachine* CDataLogStateMachine::mspDataLogStateMachine = NULL;
static const char* TAG = "LOG:";

CDataLogStateMachine::CDataLogStateMachine(void)
    : mLogState(LogStateInactive)
{
    mQueueHdl = xQueueCreate(100, sizeof(SLogData));
    mpFileStorage = new CSdmmc();
    //mpAccelerometer = NULL; //new CMpu6050Accelerometer();
}

void CDataLogStateMachine::CreateInstance(void)
{
    if (mspDataLogStateMachine == NULL) mspDataLogStateMachine = new CDataLogStateMachine();
}

CDataLogStateMachine* CDataLogStateMachine::GetInstance(void)
{
    return mspDataLogStateMachine;
}

CDataLogStateMachine::~CDataLogStateMachine()
{
    delete mspDataLogStateMachine;
    mspDataLogStateMachine = NULL;
    delete mpFileStorage;
    mpFileStorage = NULL;

    //delete mpAccelerometer;
    //mpAccelerometer = NULL;
}

void CDataLogStateMachine::Init()
{
    if (mpFileStorage)          mpFileStorage->Init();
    //if (mpAccelerometer)        mpAccelerometer->Init();
}

void CDataLogStateMachine::Receive()
{
    switch (mLogState.load())
    {
    case LogStateCreateFile:
        {
            if (mpFileStorage->CreateFile())
            {
                // File wurde erstellt -> beginne zu Loggen
                mLogState.store(LogStateLogging);
            }
            else
            {
                // File wurde nicht erstellt -> nichts tun
                mLogState.store(LogStateInactive);
            }
        }
        break;
    case LogStateLogging:
        {
            std::string dataString;
            char newElement[64];
            SLogData logData;

            while(xQueueReceive(mQueueHdl, &logData, (TickType_t)0))
            {
                sprintf(newElement, "%7.3f,%1d,%11d,%11d,%11d,%10u\n"
                    ,logData.pos
                    ,logData.marker
                    ,logData.acceleration_data.acceleration_x
                    ,logData.acceleration_data.acceleration_y
                    ,logData.acceleration_data.acceleration_z
                    ,logData.index);
                dataString.append(newElement);
            }

            mpFileStorage->PutString(dataString);
        }
        break;
    case LogStateEndLog:
        {
            xQueueReset(mQueueHdl);
            mpFileStorage->Finish();
            ESP_LOGI(TAG, "File Written");
            mLogState.store(LogStateInactive);
        }
        break;
    case LogStateInactive:
    default:
        break;
    }
}

void CDataLogStateMachine::Send()
{
    static SLogData logData;

    switch (mLogState.load())
    {
    case LogStateInactive:
    case LogStateCreateFile:
    case LogStateEndLog:
        {
            logData.index = 0;
        }
        break;
    case LogStateLogging:
        {
            logData.index++;

            //if (mpAccelerometer)        logData.acceleration_data  = mpAccelerometer->GetAcceleration();

            if(mMarker.load())
            {
                logData.marker = true;
                mMarker.store(false);
            }
            else
            {
                logData.marker = false;
            }

            if (mQueueHdl)
            {
                xQueueSend(mQueueHdl, (void *)&logData, (TickType_t) 0);
            }
        }
        break;
    default: break;
    }
}