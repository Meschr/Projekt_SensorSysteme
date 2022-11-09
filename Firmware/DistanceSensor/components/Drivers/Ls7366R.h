#ifndef _LS7366R_hf
#define _LS7366R_hf

#include "driver/spi_master.h"
#include "IIncrementalEncoder.h"

class CLs7366r : public IIncrementalEncoder
{
public:
    CLs7366r(unsigned int IncPerRev, unsigned int QuadratureMode) : IIncrementalEncoder(IncPerRev,QuadratureMode) {}
    ~CLs7366r(void) {}

    // Initialisierung des Chips
    virtual void Init(void);

    // Auslesen der gezaehlten Inkremente
    virtual int ReadCnt(void);

    //Counter mit Inkremente nullen
    virtual void ResetCnt(void);

private:
    spi_device_handle_t mSpiDeviceHdl;
};

#endif // #ifndef _LS7366R_hf