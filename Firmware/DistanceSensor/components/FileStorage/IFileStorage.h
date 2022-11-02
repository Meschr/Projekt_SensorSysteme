#ifndef _IFILESTORAGE_hf
#define _IFILESTORAGE_hf

#include <string>

class IFileStorage
{
public:
    IFileStorage() {}
    virtual ~IFileStorage() {}

    virtual void Init(void) = 0;
    virtual bool CreateFile(void) = 0;
    virtual void PutString(const std::string& in_dataString) = 0;
    virtual void Finish(void) = 0;
};

#endif // #ifndef _IFILESTORAGE_hf