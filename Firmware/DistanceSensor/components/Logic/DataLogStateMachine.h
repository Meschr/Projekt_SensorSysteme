#ifndef _DATALOGSTATEMACHINE_hf
#define _DATALOGSTATEMACHINE_hf

#include <atomic>
#include <fstream>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "LogData.h"
#include "IFileStorage.h"
#include "IPositionMeasurement.h"
#include "IInertialMeasurementUnit.h"

class CDataLogStateMachine
{
public:
    enum ELogState
    {
        LogStateInactive = 0,
        LogStateCreateFile,
        LogStateLogging,
        LogStateEndLog
    };

    static void CreateInstance(void);
    static CDataLogStateMachine* GetInstance(void);
    ~CDataLogStateMachine(void);

    void Init(void);
    void Receive(void);
    void Send(void);

    void StartLogging(void) { mLogState.store(LogStateCreateFile); }
    void StopLogging(void) { IsLogging() ? mLogState.store(LogStateEndLog) : mLogState.store(LogStateInactive); }
    bool IsLogging(void) { return (mLogState.load() == LogStateLogging); }
    void SetMarker(void) { mMarker.store(true); }

private:
    CDataLogStateMachine(void);
    CDataLogStateMachine(CDataLogStateMachine const&);  // Don't implement
    void operator=(CDataLogStateMachine const&);        // Don't implement

    static CDataLogStateMachine* mspDataLogStateMachine;

    QueueHandle_t               mQueueHdl;
    IFileStorage*               mpFileStorage;
    IPositionMeasurement*       mpPositionMeasurement;
    //IInertialMeasurementUnit*   mpImuUnten;
    //IInertialMeasurementUnit*   mpImuOben;
    std::atomic<ELogState>      mLogState;
    std::atomic_bool            mMarker;
};

#endif // #ifndef _DATALOGSTATEMACHINE_hf