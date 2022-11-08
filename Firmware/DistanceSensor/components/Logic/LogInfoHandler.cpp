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

    return dataString;
}