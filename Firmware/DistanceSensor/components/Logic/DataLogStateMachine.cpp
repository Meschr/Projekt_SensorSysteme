#include "DataLogStateMachine.h"
#include <sys/stat.h>
#include "driver/sdmmc_host.h"
#include "esp_log.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

#include "LS7366R.h"
#include "Mpu6050.h"
#include "Sdmmc.h"

CDataLogStateMachine* CDataLogStateMachine::mspDataLogStateMachine = NULL;
static const char* TAG = "LOG";

CDataLogStateMachine::CDataLogStateMachine(void)
    : mLogState(LogStateInactive)
{
    mQueueHdl = xQueueCreate(100, sizeof(SLogData));
    mpFileStorage = new CSdmmc();
    const unsigned int inc2Mm = 100;
    mpPositionMeasurement = new CLs7366r(inc2Mm);
    mCMpu6050 = new CMpu6050(MPU6050_ADDRESS_LOW, ACCEL_FULL_SCALE_RANGE_4, GYRO_FULL_SCALE_RANGE_250);
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

    delete mpPositionMeasurement;
    mpPositionMeasurement = NULL;

    //delete mpAccelerometer;
    //mpAccelerometer = NULL;
}

void CDataLogStateMachine::Init()
{
    if (mpFileStorage)          mpFileStorage->Init();
    if (mpPositionMeasurement)  mpPositionMeasurement->Init();
    //if (mpAccelerometer)        mpAccelerometer->Init();
    if(mCMpu6050)               mCMpu6050->Init();
    auto init_success = mCMpu6050->TestConnection() ? "mpu6050 initialized successfully!" : "mpu6050 test connection failed!";
    ESP_LOGI(TAG, "%s",init_success);
    auto temp = mCMpu6050->GetTemperature();
    ESP_LOGI(TAG, "Temperature: %f °C",temp);
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
            if (mpPositionMeasurement)  logData.pos                = mpPositionMeasurement->GetPositionMm();
            //if (mpAccelerometer)        logData.acceleration_data  = mpAccelerometer->GetAcceleration();
            mCMpu6050->GetAndConvertAccelerationX();

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