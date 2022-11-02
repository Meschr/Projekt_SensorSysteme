#ifndef _SDMMC_hf
#define _SDMMC_hf

#include <fstream>
#include "IFileStorage.h"

class CSdmmc : public IFileStorage
{
public:
    CSdmmc() : IFileStorage() {}
    ~CSdmmc() {}

    virtual void Init(void);
    virtual bool CreateFile(void);
    virtual void PutString(const std::string& in_dataString);
    virtual void Finish(void);

private:
    std::ofstream mOfstream;
};

#endif // #ifndef _SDMMC_hf