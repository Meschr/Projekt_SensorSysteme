#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp32_i2c_rw.h"

#define PIN_NUM_SDA 21
#define PIN_NUM_SCL 22

CI2cBusHandler* gpI2cBusHandler = NULL;

CI2cBusHandler* CI2cBusHandler::GetInstance(void)
{
    if (gpI2cBusHandler == NULL) gpI2cBusHandler = new CI2cBusHandler();
    return gpI2cBusHandler;
}

CI2cBusHandler::CI2cBusHandler(void)
{
	mI2cPortNum = I2C_NUM_0;
	mBusFrequency = 800*1000;

	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = PIN_NUM_SDA; 			 // select GPIO specific to your project
	conf.scl_io_num = PIN_NUM_SCL; 			 // select GPIO specific to your project
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = mBusFrequency;   // select frequency specific to your project
	conf.clk_flags = 0;
	esp_err_t err = i2c_param_config(mI2cPortNum, &conf);
	ESP_ERROR_CHECK(err);

	err = i2c_driver_install(mI2cPortNum, I2C_MODE_MASTER, 0,0,0);
	ESP_ERROR_CHECK(err);

	mBusIsInitialized = true;
	ESP_LOGI(TAG, "Bus driver installed sucessfully!");
}

CI2cBusHandler::~CI2cBusHandler(void)
{
    delete gpI2cBusHandler;
    gpI2cBusHandler = NULL;
}

int CI2cBusHandler::GetClockFrequencyInHz()
{
	return mBusFrequency;
}

void CI2cBusHandler::SetClockFrequencyInHz(int clockSpeed)
{
	mBusFrequency = clockSpeed;
}

void CI2cBusHandler::select_register(uint8_t device_address, uint8_t register_address)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, 1);
	i2c_master_write_byte(cmd, register_address, 1);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(mI2cPortNum, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}

int8_t CI2cBusHandler::esp32_i2c_read_bytes
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t size,
	uint8_t* data
)
{
	CI2cBusHandler::select_register(device_address, register_address);
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_READ, 1);

	if (size > 1)
		i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);

	i2c_master_read_byte(cmd, data + size - 1, I2C_MASTER_NACK);

	i2c_master_stop(cmd);
	i2c_master_cmd_begin(mI2cPortNum, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	return (size);
}

int8_t CI2cBusHandler::esp32_i2c_read_byte
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t* data
)
{
	return (CI2cBusHandler::esp32_i2c_read_bytes(device_address, register_address, 1, data));
}

int8_t CI2cBusHandler::esp32_i2c_read_bits
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t bit_start,
	uint8_t size,
	uint8_t* data
)
{
	uint8_t bit;
	uint8_t count;

	if ((count = CI2cBusHandler::esp32_i2c_read_byte(device_address, register_address, &bit))) {
		uint8_t mask = ((1 << size) - 1) << (bit_start - size + 1);

		bit &= mask;
		bit >>= (bit_start - size + 1);
		*data = bit;
	}

	return (count);
}

int8_t CI2cBusHandler::esp32_i2c_read_bit
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t bit_number,
	uint8_t* data
)
{
	uint8_t bit;
	uint8_t count = CI2cBusHandler::esp32_i2c_read_byte(device_address, register_address, &bit);

	*data = bit & (1 << bit_number);

	return (count);
}

bool CI2cBusHandler::esp32_i2c_write_bytes
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t size,
	uint8_t* data
)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, 1);
	i2c_master_write_byte(cmd, register_address, 1);
	i2c_master_write(cmd, data, size - 1, 0);
	i2c_master_write_byte(cmd, data [size - 1], 1);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(mI2cPortNum, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	return (true);
}

bool CI2cBusHandler::esp32_i2c_write_byte
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t data
)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, 1);
	i2c_master_write_byte(cmd, register_address, 1);
	i2c_master_write_byte(cmd, data, 1);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(mI2cPortNum, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	return (true);
}

bool CI2cBusHandler::esp32_i2c_write_bits
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t bit_start,
	uint8_t size,
	uint8_t data
)
{
	uint8_t bit = 0;

	if (CI2cBusHandler::esp32_i2c_read_byte(device_address, register_address, &bit) != 0) {
		uint8_t mask = ((1 << size) - 1) << (bit_start - size + 1);
		data <<= (bit_start - size + 1);
		data &= mask;
		bit &= ~(mask);
		bit |= data;
		return (CI2cBusHandler::esp32_i2c_write_byte(device_address, register_address, bit));
	}
	else
		return (false);
}

bool CI2cBusHandler::esp32_i2c_write_bit
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t bit_number,
	uint8_t data
)
{
	uint8_t bit;

	CI2cBusHandler::esp32_i2c_read_byte(device_address, register_address, &bit);

	if (data != 0)
		bit = (bit | (1 << bit_number));
	else
		bit = (bit & ~(1 << bit_number));

	return (CI2cBusHandler::esp32_i2c_write_byte(device_address, register_address, bit));
}

int8_t CI2cBusHandler::esp32_i2c_write_word
(
	uint8_t device_address,
	uint8_t register_address,
	uint8_t data
)
{
	uint8_t data_1[] = {(uint8_t) (data >> 8), (uint8_t) (data & 0xFF)};

	CI2cBusHandler::esp32_i2c_write_bytes(device_address, register_address, 2, data_1);

	return (1);
}