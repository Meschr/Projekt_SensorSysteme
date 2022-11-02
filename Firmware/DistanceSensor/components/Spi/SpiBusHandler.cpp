#include "SpiBusHandler.h"
#include <esp_system.h>
#include <esp_log.h>
#include <esp_err.h>

#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_CLK 18

CSpiBusHandler* gpSpiBusHandler = NULL;

CSpiBusHandler* CSpiBusHandler::GetInstance(void)
{
    if (gpSpiBusHandler == NULL) gpSpiBusHandler = new CSpiBusHandler();
    return gpSpiBusHandler;
}

CSpiBusHandler::CSpiBusHandler(void)
{
    esp_err_t err;
    spi_bus_config_t spi_bus_cfg = 
    {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0 // maximum transfer size in bytes (Default is 4094 if 0)
    };
    err = spi_bus_initialize(VSPI_HOST, &spi_bus_cfg,0);
    ESP_ERROR_CHECK(err);
}

CSpiBusHandler::~CSpiBusHandler(void)
{
    delete gpSpiBusHandler;
    gpSpiBusHandler = NULL;
}

void CSpiBusHandler::AddDevice(spi_device_interface_config_t spiDeviceCfg, spi_device_handle_t& rSpiDeviceHandle)
{
    esp_err_t err;
    err = spi_bus_add_device(VSPI_HOST, &spiDeviceCfg, &rSpiDeviceHandle);
    ESP_ERROR_CHECK(err);
}