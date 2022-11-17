#ifndef _LogInfo_hf
#define _LogInfo_hf

#include <string>

struct LogInfo
{
    int SamplingFreq;
    float accelBias[3];
    float gyroBias[3];
};

class CLogInfoHandler
{
public: 
    static CLogInfoHandler* GetInstance(void);
    ~CLogInfoHandler(void);

    std::string GetLogInfo(void);
    LogInfo mLogInfo;

private:
    CLogInfoHandler(void);
    CLogInfoHandler(CLogInfoHandler const&);  // Don't implement
    void operator=(CLogInfoHandler const&);   // Don't implement

    static CLogInfoHandler* msLogInfoHandler;
};

#endif // #ifndef _LogInfo_hf