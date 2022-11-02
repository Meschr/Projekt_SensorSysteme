#ifndef _SPIBUSHANDLER_hf
#define _SPIBUSHANDLER_hf

#include "driver/spi_master.h"

class CSpiBusHandler
{
public:
    static CSpiBusHandler* GetInstance(void);
    ~CSpiBusHandler(void);
    void AddDevice(spi_device_interface_config_t spiDeviceCfg, spi_device_handle_t& rSpiDeviceHandle);

private:
    CSpiBusHandler(void);
    CSpiBusHandler(CSpiBusHandler const&);  // Don't implement
    void operator=(CSpiBusHandler const&);  // Don't implement
};

#endif // #ifndef _SPIBUSHANDLER_hf