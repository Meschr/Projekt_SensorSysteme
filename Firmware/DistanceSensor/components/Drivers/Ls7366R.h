#ifndef _LS7366R_hf
#define _LS7366R_hf

#include "driver/spi_master.h"
#include "IIncrementalEncoder.h"

class CLs7366r : public IIncrementalEncoder
{
public:
    CLs7366r(unsigned int in_incPerMm) : IIncrementalEncoder(in_incPerMm) {}
    ~CLs7366r(void) {}

    // Initialisierung des Chips
    virtual void Init(void);

    // Auslesen der gezaehlten Inkremente
    virtual unsigned int ReadCnt(void);

private:
    spi_device_handle_t mSpiDeviceHdl;
};

#endif // #ifndef _LS7366R_hf