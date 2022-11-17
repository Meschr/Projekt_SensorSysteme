#include "LogInfoHandler.h"

CLogInfoHandler* CLogInfoHandler::msLogInfoHandler = NULL;

CLogInfoHandler* CLogInfoHandler::GetInstance(void)
{
    if (msLogInfoHandler == NULL) msLogInfoHandler = new CLogInfoHandler();
    return msLogInfoHandler;
}

CLogInfoHandler::CLogInfoHandler()
{
    mLogInfo.SamplingFreq = 0;
}

CLogInfoHandler::~CLogInfoHandler(void)
{
    delete msLogInfoHandler;
    msLogInfoHandler = NULL;
}

std::string CLogInfoHandler::GetLogInfo(void)
{
    std::string dataString = "Sampling freqeuncy: " + std::to_string(mLogInfo.SamplingFreq) + " Hz\n";
    dataString += "accel bias x: " +  std::to_string(mLogInfo.accelBias[0]) + "|" + "accel bias y: " +  std::to_string(mLogInfo.accelBias[1]) + "|" + "accel bias z: " +  std::to_string(mLogInfo.accelBias[2]) + "\n";
    return dataString;
}