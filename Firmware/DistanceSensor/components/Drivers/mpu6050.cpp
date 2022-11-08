#include "Mpu6050.h"
#include <string.h>

#define PI              (3.14159265358979323846f)
#define GYRO_MEAS_ERROR (PI * (60.0f / 180.0f))
#define GYRO_MEAS_DRIFT (PI * (1.0f / 180.0f))
#define BETA            (sqrt(3.0f / 4.0f) * GYRO_MEAS_ERROR)
#define ZETA            (sqrt(3.0f / 4.0f) * GYRO_MEAS_DRIFT)

float quart[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float delta_t = 0.0f;
float pitch, yaw, roll;
int last_update = 0, first_update = 0, now = 0;

uint8_t buffer[14];

CMpu6050::CMpu6050(EMpu6050_Address address, EAccelFullScaleRange accelRange, EGyroFullScaleRange gyroRange) 
{
    i2cBus = CI2cBusHandler::GetInstance();
    deviceAddress = address;
    currentAccelRange = accelRange;
    currentGyroRange = gyroRange;
    TAG_MPU6050 = address == MPU6050_ADDRESS_LOW ? "MPU6050(0x68)" : "MPU6050(0x69)";
}

CMpu6050::~CMpu6050(void)
{
    delete i2cBus;
    i2cBus = NULL;
}

void CMpu6050::Init()
{
    SetClockSource(MPU6050_CLOCK_PLL_XGYRO);
    SetFullScaleGyroRange(currentGyroRange);
    SetFullScaleAccelRange(currentAccelRange);
    SetSleepEnabled(0);
}

bool CMpu6050::TestConnection()
{
    return (GetDeviceId() == 0x34);
}

const char* CMpu6050::GetTag()
{
    return (TAG_MPU6050);
}

uint8_t CMpu6050::GetAuxVddioLevel()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_YG_OFFS_TC,
        MPU6050_TC_PWR_MODE_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetAuxVddioLevel(uint8_t level)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_YG_OFFS_TC,
        MPU6050_TC_PWR_MODE_BIT,
        level
    );
}

uint8_t CMpu6050::GetOutputRate()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SMPLRT_DIV,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetOutputRate(uint8_t rate)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SMPLRT_DIV,
        rate
    );
}

uint8_t CMpu6050::GetExternalFrameSync()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_CONFIG,
        MPU6050_CFG_EXT_SYNC_SET_BIT,
        MPU6050_CFG_EXT_SYNC_SET_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetExternalFrameSync(uint8_t sync)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_CONFIG,
        MPU6050_CFG_EXT_SYNC_SET_BIT,
        MPU6050_CFG_EXT_SYNC_SET_LENGTH,
        sync
    );
}

uint8_t CMpu6050::GetDlpfMode()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_CONFIG,
        MPU6050_CFG_DLPF_CFG_BIT,
        MPU6050_CFG_DLPF_CFG_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetDlpfMode(uint8_t mode)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_CONFIG,
        MPU6050_CFG_DLPF_CFG_BIT,
        MPU6050_CFG_DLPF_CFG_LENGTH,
        mode
    );
}

uint8_t CMpu6050::GetFullScaleGyroRange()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_GYRO_CONFIG,
        MPU6050_GCONFIG_FS_SEL_BIT,
        MPU6050_GCONFIG_FS_SEL_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFullScaleGyroRange(uint8_t range)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_GYRO_CONFIG,
        MPU6050_GCONFIG_FS_SEL_BIT,
        MPU6050_GCONFIG_FS_SEL_LENGTH,
        range
    );
}

uint8_t CMpu6050::GetAccelXSelfTestFactoryTrim()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_X,
        &buffer[0]
    );
	i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_A,
        &buffer[1]
    );

    return ((buffer[0] >> 3) | ((buffer[1] >> 4) & 0x03));
}

uint8_t CMpu6050::GetAccelYSelfTestFactoryTrim()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_Y,
        &buffer[0]
    );
	i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_A,
        &buffer[1]
    );

    return ((buffer[0] >> 3) | ((buffer[1] >> 2) & 0x03));
}

uint8_t CMpu6050::GetAccelZSelfTestFactoryTrim()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_Z,
        2,
        buffer
    );

    return ((buffer[0] >> 3) | (buffer[1] & 0x03));
}

uint8_t CMpu6050::GetGyroXSelfTestFactoryTrim()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_X,
        buffer
    );

    return ((buffer[0] & 0x1F));
}

uint8_t CMpu6050::GetGyroYSelfTestFactoryTrim()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_Y,
        buffer
    );

    return ((buffer[0] & 0x1F));
}

uint8_t CMpu6050::GetGyroZSelfTestFactoryTrim()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_SELF_TEST_Z,
        buffer
    );

    return ((buffer[0] & 0x1F));
}

bool CMpu6050::GetAccelXSelfTest()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_XA_ST_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetAccelXSelfTest(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_XA_ST_BIT,
        enabled
    );
}

bool CMpu6050::GetAccelYSelfTest()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_YA_ST_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetAccelYSelfTest(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_YA_ST_BIT,
        enabled
    );
}

bool CMpu6050::GetAccelZSelfTest()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_ZA_ST_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetAccelZSelfTest(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_ZA_ST_BIT,
        enabled
    );
}

uint8_t CMpu6050::GetFullScaleAccelRange()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_AFS_SEL_BIT,
        MPU6050_ACONFIG_AFS_SEL_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFullScaleAccelRange(uint8_t range)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH,
        range);
}

uint8_t CMpu6050::GetDhpfMode()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_ACCEL_HPF_BIT, MPU6050_ACONFIG_ACCEL_HPF_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetDhpfMode(uint8_t mode)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_CONFIG,
        MPU6050_ACONFIG_ACCEL_HPF_BIT,
        MPU6050_ACONFIG_ACCEL_HPF_LENGTH,
        mode
    );
}

uint8_t CMpu6050::GetFreefallDetectionThreshold()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_FF_THR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFreefallDetectionThreshold(uint8_t threshold)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_FF_THR,
        threshold
    );
}

uint8_t CMpu6050::GetFreefallDetectionDuration()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_FF_DUR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFreefallDetectionDuration(uint8_t duration)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_FF_DUR,
        duration
    );
}

uint8_t CMpu6050::GetMotionDetectionThreshold()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_THR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetMotionDetectionThreshold(uint8_t threshold)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_THR,
        threshold
    );
}

uint8_t CMpu6050::GetMotionDetectionDuration()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DUR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetMotionDetectionDuration(uint8_t duration)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DUR,
        duration
    );
}

uint8_t CMpu6050::GetZeroMotionDetectionThreshold()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_ZRMOT_THR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetZeroMotionDetectionThreshold(uint8_t threshold)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_ZRMOT_THR,
        threshold
    );
}

uint8_t CMpu6050::GetZeroMotionDetectionDuration()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_ZRMOT_DUR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetZeroMotionDetectionDuration(uint8_t duration)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_ZRMOT_DUR,
        duration
    );
}

bool CMpu6050::GetTempFifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_TEMP_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetTempFifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_TEMP_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetXGyroFifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_XG_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetXGyroFifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_XG_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetYGyroFifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_YG_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetYGyroFifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_YG_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetZGyroFifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_ZG_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetZGyroFifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_ZG_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetAccelFifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_ACCEL_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetAccelFifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_ACCEL_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlave2FifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_SLV2_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave2FifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_SLV2_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlave1FifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_SLV1_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave1FifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_SLV1_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlave0FifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_SLV0_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave0FifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_EN,
        MPU6050_SLV0_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetMultiMasterEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_MULT_MST_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetMultiMasterEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_MULT_MST_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetWaitForExternalSensorEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_WAIT_FOR_ES_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetWaitForExternalSensorEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_WAIT_FOR_ES_BIT,
        enabled
    );
}

bool CMpu6050::GetSlave3FifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_SLV_3_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave3FifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_SLV_3_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlaveReadWriteTransitionEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_I2C_MST_P_NSR_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveReadWriteTransitionEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_I2C_MST_P_NSR_BIT,
        enabled
    );
}

uint8_t CMpu6050::GetMasterClockSpeed()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_I2C_MST_CLK_BIT,
        MPU6050_I2C_MST_CLK_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetMasterClockSpeed(uint8_t speed)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_CTRL,
        MPU6050_I2C_MST_CLK_BIT,
        MPU6050_I2C_MST_CLK_LENGTH,
        speed
    );
}

uint8_t CMpu6050::GetSlaveAddress(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_ADDR + num * 3,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveAddress(uint8_t num, uint8_t address)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_ADDR + num * 3,
        address
    );
}

uint8_t CMpu6050::GetSlaveRegister(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_REG + num * 3,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveRegister(uint8_t num, uint8_t reg)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_REG + num * 3,
        reg
    );
}

bool CMpu6050::GetSlaveEnabled(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveEnabled(uint8_t num, bool enabled)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlaveWordByteSwap(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_BYTE_SW_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveWordByteSwap(uint8_t num, bool enabled)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_BYTE_SW_BIT,
        enabled
    );
}

bool CMpu6050::GetSlaveWriteMode(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_REG_DIS_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveWriteMode(uint8_t num, bool mode)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_REG_DIS_BIT,
        mode
    );
}

bool CMpu6050::GetSlaveWordGroupOffset(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_GRP_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveWordGroupOffset(uint8_t num, bool enabled)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_GRP_BIT,
        enabled
    );
}

uint8_t CMpu6050::GetSlaveDataLength(uint8_t num)
{
    if (num > 3)
        return (0);

    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_LEN_BIT,
        MPU6050_I2C_SLV_LEN_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveDataLength(uint8_t num, uint8_t length)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_CTRL + num * 3,
        MPU6050_I2C_SLV_LEN_BIT,
        MPU6050_I2C_SLV_LEN_LENGTH,
        length
    );
}

uint8_t CMpu6050::GetSlave4Address()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_ADDR,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave4Address(uint8_t address)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_ADDR,
        address
    );
}

uint8_t CMpu6050::GetSlave4Register()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_REG,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave4Register(uint8_t reg)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_REG,
        reg
    );
}

void CMpu6050::SetSlave4OutputByte(uint8_t data)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_DO,
        data
    );
}

bool CMpu6050::GetSlave4Enabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave4Enabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlave4TnterruptEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_INT_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave4InterruptEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_INT_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetSlave4WriteMode()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_REG_DIS_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave4WriteMode(bool mode)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_REG_DIS_BIT,
        mode
    );
}

uint8_t CMpu6050::GetSlave4MasterDelay()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_MST_DLY_BIT,
        MPU6050_I2C_SLV4_MST_DLY_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlave4MasterDelay(uint8_t delay)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_CTRL,
        MPU6050_I2C_SLV4_MST_DLY_BIT,
        MPU6050_I2C_SLV4_MST_DLY_LENGTH,
        delay
    );
}

uint8_t CMpu6050::GetSlave4InputByte()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV4_DI,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetPassthroughStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_PASS_THROUGH_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetSlave4IsDone()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_SLV4_DONE_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetLostArbitration()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_LOST_ARB_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetSlave4Nack()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_SLV4_NACK_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetSlave3Nack()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_SLV3_NACK_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetSlave2Nack()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_SLV2_NACK_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetSlave1Nack()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_SLV1_NACK_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetSlave0Nack()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_STATUS,
        MPU6050_MST_I2C_SLV0_NACK_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetInterruptMode()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_INT_LEVEL_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetInterruptMode(bool mode)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_INT_LEVEL_BIT,
        mode
    );
}

bool CMpu6050::GetInterruptDrive()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_INT_OPEN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetInterruptDrive(bool drive)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_INT_OPEN_BIT,
        drive
    );
}

bool CMpu6050::GetInterruptLatch()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_LATCH_INT_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetInterruptLatch(bool latch)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_LATCH_INT_EN_BIT,
        latch
    );
}

bool CMpu6050::GetInterruptLatchClear()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_INT_RD_CLEAR_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetInterruptLatchClear(bool clear)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_INT_RD_CLEAR_BIT,
        clear
    );
}

bool CMpu6050::GetFsyncInterruptLevel()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFsyncInterruptLevel(bool level)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT,
        level
    );
}

bool CMpu6050::GetFsyncInterruptEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_FSYNC_INT_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFsyncInterruptEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_FSYNC_INT_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetI2cBypassEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_I2C_BYPASS_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetI2cBypassEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_I2C_BYPASS_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetClockOutputEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_CLKOUT_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetClockOutputEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_PIN_CFG,
        MPU6050_INTCFG_CLKOUT_EN_BIT,
        enabled
    );
}

uint8_t CMpu6050::GetIntEnabled()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntEnabled(uint8_t enabled)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        enabled
    );
}

bool CMpu6050::GetIntFreefallEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_FF_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntFreefallEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_FF_BIT,
        enabled
    );
}

bool CMpu6050::GetIntMotionEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_MOT_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntMotionEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_MOT_BIT,
        enabled
    );
}

bool CMpu6050::GetIntZeroMotionEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_ZMOT_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntZeroMotionEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_ZMOT_BIT,
        enabled
    );
}

bool CMpu6050::GetIntFifoBufferOverflowEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_FIFO_OFLOW_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntFifoBufferOverflowEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_FIFO_OFLOW_BIT,
        enabled
    );
}

bool CMpu6050::GetIntI2cMasterEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_I2C_MST_INT_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntI2cMasterEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_I2C_MST_INT_BIT,
        enabled
    );
}

bool CMpu6050::GetIntDataReadyEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_DATA_RDY_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntDataReadyEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_DATA_RDY_BIT,
        enabled
    );
}

uint8_t CMpu6050::GetIntStatus()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntFreefallStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_FF_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntMotionStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_MOT_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntZeroMotionStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_ZMOT_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntFifoBufferOverflowStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_FIFO_OFLOW_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntI2cMasterStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_I2C_MST_INT_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntDataReadyStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_DATA_RDY_BIT,
        buffer
    );

    return (buffer[0]);
}

SAccelerationData CMpu6050::GetAcceleration()
{   
    SAccelerationData data = {0};
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_XOUT_H,
        6,
        buffer
    );
    data.acceleration_x = (((int16_t) buffer[0]) << 8) | buffer[1];
    data.acceleration_y = (((int16_t) buffer[2]) << 8) | buffer[3];
    data.acceleration_z = (((int16_t) buffer[4]) << 8) | buffer[5];

    return data;
}

int16_t CMpu6050::GetAccelerationX()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_XOUT_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

int GetIntValueFromAccelRange(EAccelFullScaleRange range)
{
    switch(range)
    {
        case ACCEL_FULL_SCALE_RANGE_2:
            return 16384;
        case ACCEL_FULL_SCALE_RANGE_4:
            return 8192; 
        case ACCEL_FULL_SCALE_RANGE_8:
            return 4096;
        case ACCEL_FULL_SCALE_RANGE_16:
            return 2048;
        default:
            return 0; //TODO handle this case
    }
}

double CMpu6050::GetAndConvertAccelerationX()
{
    auto rawReading = CMpu6050::GetAccelerationX();
    double accelXInG = rawReading/static_cast<double>(GetIntValueFromAccelRange(currentAccelRange));
    ESP_LOGI(TAG_MPU6050, "Acceleration in X: %lf G",accelXInG);
    return accelXInG;        
}

int16_t CMpu6050::GetAccelerationY()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_YOUT_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

double CMpu6050::GetAndConvertAccelerationY()
{
    auto rawReading = CMpu6050::GetAccelerationY();
    double accelYIng = rawReading/static_cast<double>(GetIntValueFromAccelRange(currentAccelRange));
    ESP_LOGI(TAG_MPU6050, "Acceleration in Y: %lf G", accelYIng);
    return accelYIng;
}

int16_t CMpu6050::GetAccelerationZ()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_ZOUT_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

double CMpu6050::GetAndConvertAccelerationZ()
{
    auto rawReading = CMpu6050::GetAccelerationZ();
    double accelZIng = rawReading/static_cast<double>(GetIntValueFromAccelRange(currentAccelRange));
    ESP_LOGI(TAG_MPU6050, "Acceleration in Z: %lf G", accelZIng);
    return accelZIng;
}

int16_t CMpu6050::GetTemperature()
{
    i2cBus->esp32_i2c_read_bytes(deviceAddress, MPU6050_REGISTER_TEMP_OUT_H, 2, buffer);
    
    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

float CMpu6050::GetAndConvertTemperatureToCelsius()
{
    return (GetTemperature/340)+36.53;
}

SRotationData CMpu6050::GetRotation()
{
    SRotationData data = {0};

    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_GYRO_XOUT_H,
        6,
        buffer
    );
    data.rotation_x = (((int16_t) buffer[0]) << 8) | buffer[1];
    data.rotation_y = (((int16_t) buffer[2]) << 8) | buffer[3];
    data.rotation_z = (((int16_t) buffer[4]) << 8) | buffer[5];

    return data;
}

int16_t CMpu6050::GetRotationX()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_GYRO_XOUT_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

int16_t CMpu6050::GetRotationY()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_GYRO_YOUT_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

int16_t CMpu6050::GetRotationZ()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_GYRO_ZOUT_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::GetMotion
(
    mpu6050_acceleration_t* data_accel,
    mpu6050_rotation_t* data_gyro
)
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ACCEL_XOUT_H,
        14,
        buffer
    );

    data_accel->accel_x = (((int16_t) buffer[0]) << 8) | buffer[1];
    data_accel->accel_y = (((int16_t) buffer[2]) << 8) | buffer[3];
    data_accel->accel_z = (((int16_t) buffer[4]) << 8) | buffer[5];
    data_gyro->gyro_x = (((int16_t) buffer[8]) << 8) | buffer[9];
    data_gyro->gyro_y = (((int16_t) buffer[10]) << 8) | buffer[11];
    data_gyro->gyro_z = (((int16_t) buffer[12]) << 8) | buffer[13];
}

uint8_t CMpu6050::GetExternalSensorByte(int position)
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_EXT_SENS_DATA_00 + position, 
        buffer
    );

    return (buffer[0]);
}

uint16_t CMpu6050::GetExternalSensorWord(int position)
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_EXT_SENS_DATA_00 + position,
        2,
        buffer
    );

    return ((((uint16_t) buffer[0]) << 8) | buffer[1]);
}

uint32_t CMpu6050::GetExternalSensorDword(int position)
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_EXT_SENS_DATA_00 + position,
        4,
        buffer
    );

    return
    (
        (((uint32_t) buffer[0]) << 24) |
        (((uint32_t) buffer[1]) << 16) |
        (((uint16_t) buffer[2]) << 8) |
        buffer[3]
    );
}

uint8_t CMpu6050::GetMotionStatus()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetXNegativeMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_XNEG_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetXPositiveMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_XPOS_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetYNegativeMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_YNEG_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetYPositiveMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_YPOS_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetZNegativeMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_ZNEG_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetZPositiveMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_ZPOS_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetZeroMotionDetected()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_STATUS,
        MPU6050_MOTION_MOT_ZRMOT_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveOutputByte(uint8_t num, uint8_t data)
{
    if (num > 3)
        return;

    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_SLV0_DO + num,
        data
    );
}

bool CMpu6050::GetExternalShadowDelayEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_DELAY_CTRL,
        MPU6050_DLYCTRL_DELAY_ES_SHADOW_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetExternalShadowDelayEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_DELAY_CTRL,
        MPU6050_DLYCTRL_DELAY_ES_SHADOW_BIT,
        enabled
    );
}

bool CMpu6050::GetSlaveDelayEnabled(uint8_t num)
{
    if (num > 4)
        return (0);

    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_DELAY_CTRL,
        num,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSlaveDelayEnabled(uint8_t num, bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_I2C_MST_DELAY_CTRL,
        num,
        enabled
    );
}

void CMpu6050::ResetGyroscopePath()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_SIGNAL_PATH_RESET,
        MPU6050_PATHRESET_GYRO_RESET_BIT,
        1
    );
}

void CMpu6050::ResetAccelerometerPath()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_SIGNAL_PATH_RESET,
        MPU6050_PATHRESET_ACCEL_RESET_BIT,
        1
    );
}

void CMpu6050::ResetTemperaturePath()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_SIGNAL_PATH_RESET,
        MPU6050_PATHRESET_TEMP_RESET_BIT,
        1
    );
}

uint8_t CMpu6050::GetAccelerometerPowerOnDelay()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_CTRL,
        MPU6050_DETECT_ACCEL_DELAY_BIT,
        MPU6050_DETECT_ACCEL_DELAY_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetAccelerometerPowerOnDelay(uint8_t delay)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_CTRL,
        MPU6050_DETECT_ACCEL_DELAY_BIT,
        MPU6050_DETECT_ACCEL_DELAY_LENGTH,
        delay
    );
}

uint8_t CMpu6050::GetFreefallDetectionCounterDecrement()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_CTRL,
        MPU6050_DETECT_FF_COUNT_BIT,
        MPU6050_DETECT_FF_COUNT_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFreefallDetectionCounterDecrement(uint8_t decrement)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_CTRL,
        MPU6050_DETECT_FF_COUNT_BIT,
        MPU6050_DETECT_FF_COUNT_LENGTH,
        decrement
    );
}

uint8_t CMpu6050::GetMotionDetectionCounterDecrement()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_CTRL,
        MPU6050_DETECT_MOT_COUNT_BIT,
        MPU6050_DETECT_MOT_COUNT_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetMotionDetectionCounterDecrement(uint8_t decrement)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_MOT_DETECT_CTRL,
        MPU6050_DETECT_MOT_COUNT_BIT,
        MPU6050_DETECT_MOT_COUNT_LENGTH,
        decrement
    );
}

bool CMpu6050::GetFifoEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_FIFO_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetFifoEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_FIFO_EN_BIT,
        enabled
    );
}

bool CMpu6050::GetI2cMasterModeEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_I2C_MST_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetI2cMasterModeEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_I2C_MST_EN_BIT,
        enabled
    );
}

void CMpu6050::SwitchSpieEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_I2C_IF_DIS_BIT,
        enabled
    );
}

void CMpu6050::ResetFifo()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_FIFO_RESET_BIT,
        1
    );
}

void CMpu6050::ResetSensors()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_SIG_COND_RESET_BIT,
        1
    );
}

void CMpu6050::Reset()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_DEVICE_RESET_BIT,
        1
    );
}

bool CMpu6050::GetSleepEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_SLEEP_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetSleepEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_SLEEP_BIT,
        enabled
    );
}

bool CMpu6050::GetWakeCycleEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_CYCLE_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetWakeCycleEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_CYCLE_BIT,
        enabled
    );
}

bool CMpu6050::GetTempSensorEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_TEMP_DIS_BIT,
        buffer
    );

    return (buffer[0] == 0);
}

void CMpu6050::SetTempSensorEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_TEMP_DIS_BIT,
        !enabled
    );
}

uint8_t CMpu6050::GetClockSource()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_CLKSEL_BIT,
        MPU6050_PWR1_CLKSEL_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetClockSource(uint8_t source)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_1,
        MPU6050_PWR1_CLKSEL_BIT,
        MPU6050_PWR1_CLKSEL_LENGTH,
        source
    );
}

uint8_t CMpu6050::GetWakeFrequency()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_LP_WAKE_CTRL_BIT,
        MPU6050_PWR2_LP_WAKE_CTRL_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetWakeFrequency(uint8_t frequency)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_LP_WAKE_CTRL_BIT,
        MPU6050_PWR2_LP_WAKE_CTRL_LENGTH,
        frequency
    );
}

bool CMpu6050::GetStandbyXAccelEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_XA_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetStandbyXAccelEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_XA_BIT,
        enabled
    );
}

bool CMpu6050::GetStandbyYAccelEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_YA_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetStandbyYAccelEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_YA_BIT,
        enabled
    );
}

bool CMpu6050::GetStandbyZAccelEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_ZA_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetStandbyZAccelEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_ZA_BIT,
        enabled
    );
}

bool CMpu6050::GetStandbyXGyroEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_XG_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetStandbyXGyroEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_XG_BIT,
        enabled
    );
}

bool CMpu6050::GetStandbyYGyroEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_YG_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetStandbyYGyroEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_YG_BIT,
        enabled
    );
}

bool CMpu6050::GetStandbyZGyroEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_ZG_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetStandbyZGyroEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_PWR_MGMT_2,
        MPU6050_PWR2_STBY_ZG_BIT,
        enabled
    );
}

uint16_t CMpu6050::GetFifoCount()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_COUNTH,
        2,
        buffer
    );

    return ((((uint16_t) buffer[0]) << 8) | buffer[1]);
}

uint8_t CMpu6050::GetFifoByte()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_R_W,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::GetFifoBytes(uint8_t *data, uint8_t length)
{
    if (length > 0) {
        i2cBus->esp32_i2c_read_bytes
        (
            deviceAddress,
            MPU6050_REGISTER_FIFO_R_W,
            length,
            data
        );
    }
    else
        *data = 0;
}

void CMpu6050::SetFifoByte(uint8_t data)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_FIFO_R_W,
        data
    );
}

uint8_t CMpu6050::GetDeviceId()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_WHO_AM_I,
        MPU6050_WHO_AM_I_BIT,
        MPU6050_WHO_AM_I_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetDeviceId(uint8_t id)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_WHO_AM_I,
        MPU6050_WHO_AM_I_BIT,
        MPU6050_WHO_AM_I_LENGTH,
        id
    );
}

uint8_t CMpu6050::GetOtpBankValid()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_XG_OFFS_TC,
        MPU6050_TC_OTP_BNK_VLD_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetOtpBankValid(int8_t enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_XG_OFFS_TC,
        MPU6050_TC_OTP_BNK_VLD_BIT,
        enabled
    );
}

int8_t CMpu6050::GetXGyroOffsetTc()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_XG_OFFS_TC,
        MPU6050_TC_OFFSET_BIT,
        MPU6050_TC_OFFSET_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetXGyroOffsetTc(int8_t offset)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_XG_OFFS_TC,
        MPU6050_TC_OFFSET_BIT,
        MPU6050_TC_OFFSET_LENGTH,
        offset
    );
}

int8_t CMpu6050::GetYGyroOffsetTc()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_YG_OFFS_TC,
        MPU6050_TC_OFFSET_BIT,
        MPU6050_TC_OFFSET_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetYGyroOffsetTc(int8_t offset)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_YG_OFFS_TC,
        MPU6050_TC_OFFSET_BIT,
        MPU6050_TC_OFFSET_LENGTH,
        offset
    );
}

int8_t CMpu6050::GetZGyroOffsetTc()
{
    i2cBus->esp32_i2c_read_bits
    (
        deviceAddress,
        MPU6050_REGISTER_ZG_OFFS_TC,
        MPU6050_TC_OFFSET_BIT,
        MPU6050_TC_OFFSET_LENGTH,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetZGyroOffsetTc(int8_t offset)
{
    i2cBus->esp32_i2c_write_bits
    (
        deviceAddress,
        MPU6050_REGISTER_ZG_OFFS_TC,
        MPU6050_TC_OFFSET_BIT,
        MPU6050_TC_OFFSET_LENGTH,
        offset
    );
}

int8_t CMpu6050::GetXFineGain()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_X_FINE_GAIN,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetXFineGain(int8_t gain)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_X_FINE_GAIN,
        gain
    );
}

int8_t CMpu6050::GetYFineGain()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_Y_FINE_GAIN,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetYFineGain(int8_t gain)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_Y_FINE_GAIN,
        gain
    );
}

int8_t CMpu6050::GetZFineGain()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_Z_FINE_GAIN,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetZFineGain(int8_t gain)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_Z_FINE_GAIN,
        gain
    );
}

int16_t CMpu6050::GetXAccelOffset()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_XA_OFFS_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::SetXAccelOffset(int16_t offset)
{
    i2cBus->esp32_i2c_write_word
    (
        deviceAddress,
        MPU6050_REGISTER_XA_OFFS_H,
        offset
    );
}

int16_t CMpu6050::GetYAccelOffset()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_YA_OFFS_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::SetYAccelOffset(int16_t offset)
{
    i2cBus->esp32_i2c_write_word
    (
        deviceAddress,
        MPU6050_REGISTER_YA_OFFS_H,
        offset
    );
}

int16_t CMpu6050::GetZAccelOffset()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ZA_OFFS_H,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::SetZAccelOffset(int16_t offset)
{
    i2cBus->esp32_i2c_write_word
    (
        deviceAddress,
        MPU6050_REGISTER_ZA_OFFS_H,
        offset
    );
}

int16_t CMpu6050::GetXGyroOffset()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_XG_OFFS_USRH,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::SetXGyroOffset(int16_t offset)
{
    i2cBus->esp32_i2c_write_word
    (
        deviceAddress,
        MPU6050_REGISTER_XG_OFFS_USRH,
        offset
    );
}

int16_t CMpu6050::GetYGyroOffset()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_YG_OFFS_USRH,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::SetYGyroOffset(int16_t offset)
{
    i2cBus->esp32_i2c_write_word
    (
        deviceAddress,
        MPU6050_REGISTER_YG_OFFS_USRH,
        offset
    );
}

int16_t CMpu6050::GetZGyroOffset()
{
    i2cBus->esp32_i2c_read_bytes
    (
        deviceAddress,
        MPU6050_REGISTER_ZG_OFFS_USRH,
        2,
        buffer
    );

    return ((((int16_t) buffer[0]) << 8) | buffer[1]);
}

void CMpu6050::SetZGyroOffset(int16_t offset)
{
    i2cBus->esp32_i2c_write_word
    (
        deviceAddress,
        MPU6050_REGISTER_ZG_OFFS_USRH,
        offset
    );
}

bool CMpu6050::GetIntPllReadyEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_PLL_RDY_INT_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntPllReadyEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_PLL_RDY_INT_BIT,
        enabled
    );
}

bool CMpu6050::GetIntDmpEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_DMP_INT_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetIntDmpEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_ENABLE,
        MPU6050_INTERRUPT_DMP_INT_BIT,
        enabled
    );
}

bool CMpu6050::GetDmpInt5Status()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_INT_STATUS,
        MPU6050_DMPINT_5_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetDmpInt4Status()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_INT_STATUS,
        MPU6050_DMPINT_4_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetDmpInt3Status()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_INT_STATUS,
        MPU6050_DMPINT_3_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetDmpInt2Status()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_INT_STATUS,
        MPU6050_DMPINT_2_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetDmpInt1Status()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_INT_STATUS,
        MPU6050_DMPINT_1_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetDmpInt0Status()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_INT_STATUS,
        MPU6050_DMPINT_0_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntPplReadyStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_PLL_RDY_INT_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetIntDmpStatus()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_INT_STATUS,
        MPU6050_INTERRUPT_DMP_INT_BIT,
        buffer
    );

    return (buffer[0]);
}

bool CMpu6050::GetDmpEnabled()
{
    i2cBus->esp32_i2c_read_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_DMP_EN_BIT,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetDmpEnabled(bool enabled)
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_DMP_EN_BIT,
        enabled
    );
}

void CMpu6050::ResetDmp()
{
    i2cBus->esp32_i2c_write_bit
    (
        deviceAddress,
        MPU6050_REGISTER_USER_CTRL,
        MPU6050_USERCTRL_DMP_RESET_BIT,
        1
    );
}

uint8_t CMpu6050::GetDmpConfig1()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_CFG_1,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetDmpConfig1(uint8_t config)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_CFG_1,
        config
    );
}

uint8_t CMpu6050::GetDmpConfig2()
{
    i2cBus->esp32_i2c_read_byte
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_CFG_2,
        buffer
    );

    return (buffer[0]);
}

void CMpu6050::SetDmpConfig2(uint8_t config)
{
    i2cBus->esp32_i2c_write_byte
    (
        deviceAddress,
        MPU6050_REGISTER_DMP_CFG_2,
        config
    );
}

float CMpu6050::GetAccelRes(uint8_t accel_scale)
{
    float accel_res = 0;

    switch (accel_scale) {
        case 0:
            accel_res = 2.0 / 32768.0;
            break;
        case 1:
            accel_res = 4.0 / 32768.0;
            break;
        case 2:
            accel_res = 8.0 / 32768.0;
            break;
        case 3:
            accel_res = 16.0 / 32768.0;
            break;
    }

    return (accel_res);
}

float CMpu6050::GetGyroRes(uint8_t gyro_scale)
{
    float gyro_res = 0;
    
    switch (gyro_scale) {
        case 0:
            gyro_res = 250.0 / 32768.0;
            break;
        case 1:
            gyro_res = 500.0 / 32768.0;
            break;
        case 2:
            gyro_res = 1000.0 / 32768.0;
            break;
        case 3:
            gyro_res = 2000.0 / 32768.0;
            break;
    }

    return (gyro_res);
}

void CMpu6050::Calibrate(float *accel_bias_res, float *gyro_bias_res)
{
    int32_t accel_bias[3] = {0, 0, 0};
    int32_t gyro_bias[3] = {0, 0, 0};
    int32_t accel_bias_reg[3] = {0, 0, 0};
    uint16_t accel_temp[3] = {0, 0, 0};
    uint16_t gyro_temp[3] = {0, 0, 0};
    uint8_t mask_bit[3] = {0, 0, 0};
    uint32_t mask = 1uL;
    uint16_t gyro_sensitivity = 131;
    uint16_t accel_sensitivity = 16384;
    uint8_t tmp_data[12];
    uint16_t packet_count;
  
    Reset();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    SetClockSource(MPU6050_CLOCK_PLL_XGYRO);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  
    // Configure device for bias calculation:
    SetIntEnabled(0);
    SetFifoEnabled(0);
    SetAccelFifoEnabled(0);
    SetZGyroFifoEnabled(0);
    SetYGyroFifoEnabled(0);
    SetXGyroFifoEnabled(0);
    SetTempFifoEnabled(0);
    SetClockSource(MPU6050_CLOCK_INTERNAL);
    SetMultiMasterEnabled(0);
    SetFifoEnabled(0);
    SetI2cMasterModeEnabled(0);
    ResetSensors();
    vTaskDelay(15 / portTICK_PERIOD_MS);
  
    // Configure MPU6050 gyro and accelerometer for bias calculation:
    SetOutputRate(0x00); // Set sample rate to 1 kHz.
    SetDlpfMode(MPU6050_DLPF_BW_188);
    SetFullScaleAccelRange(ACCEL_FULL_SCALE_RANGE_2);
    SetFullScaleGyroRange(GYRO_FULL_SCALE_RANGE_250);
 
    /**
     * Configure FIFO to capture data for bias calculation.
     */

    // Enable gyroscope and accelerometer sensors for FIFO:
    SetFifoEnabled(1);
    SetAccelFifoEnabled(1);
    SetZGyroFifoEnabled(1);
    SetYGyroFifoEnabled(1);
    SetXGyroFifoEnabled(1);
    vTaskDelay(80 / portTICK_PERIOD_MS); // Accumulate 80 samples in 80 ms.
 
    // At end of sample accumulation, turn off FIFO sensor read:
    SetFifoEnabled(0);
    SetAccelFifoEnabled(0);
    SetZGyroFifoEnabled(0);
    SetYGyroFifoEnabled(0);
    SetXGyroFifoEnabled(0);
    SetTempFifoEnabled(0); 
  
    // Sets of full gyro and accelerometer data for averaging:
    packet_count = GetFifoCount() / 12;
 
    for (int i = 0; i < packet_count; i++) {
        // Read data for averaging:
        GetFifoBytes(&tmp_data[0], 6);
        accel_temp[0] = (int16_t) (((int16_t) tmp_data[0] << 8) | tmp_data[1]);
        accel_temp[1] = (int16_t) (((int16_t) tmp_data[2] << 8) | tmp_data[3]);
        accel_temp[2] = (int16_t) (((int16_t) tmp_data[4] << 8) | tmp_data[5]);
        gyro_temp[0] = (int16_t) (((int16_t) tmp_data[6] << 8) | tmp_data[7]);
        gyro_temp[1] = (int16_t) (((int16_t) tmp_data[8] << 8) | tmp_data[9]);
        gyro_temp[2] = (int16_t) (((int16_t) tmp_data[10] << 8) | tmp_data[11]);
        
        // Sum individual 16-bit biases to get accumulated signed 32-bit biases:
        accel_bias[0] += (int32_t) accel_temp[0];
        accel_bias[1] += (int32_t) accel_temp[1];
        accel_bias[2] += (int32_t) accel_temp[2];
        gyro_bias[0] += (int32_t) gyro_temp[0];
        gyro_bias[1] += (int32_t) gyro_temp[1];
        gyro_bias[2] += (int32_t) gyro_temp[2];
    }

    // Normalize sums to get average count biases:
    accel_bias[0] /= (int32_t) packet_count;
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0] /= (int32_t) packet_count;
    gyro_bias[1] /= (int32_t) packet_count;
    gyro_bias[2] /= (int32_t) packet_count;
    
    // Remove gravity from the z-axis accelerometer bias calculation:
    if (accel_bias[2] > 0L)
        accel_bias[2] -= (int32_t) accel_sensitivity;
    else
        accel_bias[2] += (int32_t) accel_sensitivity;
 
    /**
     * Construct the gyro biases for push to the hardware gyro bias registers,
     * which are reset to zero upon device startup:
     */

    // Divide by 4 to get 32.9 LSB per deg/s to expected bias input format.
    // Biases are additive, so change sign on calculated average gyro biases.
    tmp_data[0] = (-gyro_bias[0] / 4  >> 8) & 0xFF;
    tmp_data[1] = (-gyro_bias[0] / 4) & 0xFF; 
    tmp_data[2] = (-gyro_bias[1] / 4  >> 8) & 0xFF;
    tmp_data[3] = (-gyro_bias[1] / 4) & 0xFF;
    tmp_data[4] = (-gyro_bias[2] / 4  >> 8) & 0xFF;
    tmp_data[5] = (-gyro_bias[2] / 4) & 0xFF;
 
    // Push gyro biases to hardware registers:
    SetXGyroOffset(((int16_t) tmp_data[0]) << 8 | tmp_data[1]);
    SetYGyroOffset(((int16_t) tmp_data[2]) << 8 | tmp_data[3]);
    SetZGyroOffset(((int16_t) tmp_data[4]) << 8 | tmp_data[5]);
 
    // Construct gyro bias in deg/s for later manual subtraction:
    gyro_bias_res[0] = (float) gyro_bias[0] / (float) gyro_sensitivity;
    gyro_bias_res[1] = (float) gyro_bias[1] / (float) gyro_sensitivity;
    gyro_bias_res[2] = (float) gyro_bias[2] / (float) gyro_sensitivity;

    /**
     * Construct the accelerometer biases for push to the hardware accelerometer
     * bias registers. These registers contain factory trim values which must be
     * added to the calculated accelerometer biases; on boot up these registers
     * will hold non-zero values. In addition, bit 0 of the lower byte must be
     * preserved since it is used for temperature compensation calculations.
     * Accelerometer bias registers expect bias input as 2048 LSB per g, so that
     * the accelerometer biases calculated above must be divided by 8.
     */

    // Read factory accelerometer trim values:
    tmp_data[0] = GetXAccelOffset();
    tmp_data[1] = GetYAccelOffset();
    tmp_data[2] = GetZAccelOffset();
  
    for (int i = 0; i < 3; i++) {
        // If temperature compensation bit is set, record that in mask_bit:
        if (accel_bias_reg[i] & mask)
            mask_bit[i] = 0x01;
    }
 
    /**
     * Construct total accelerometer bias, including calculated average
     * accelerometer bias from above (Subtract calculated averaged accelerometer
     * bias scaled to 2048 LSB/g (16g full scale).
     */

    accel_bias_reg[0] -= (accel_bias[0] / 8);
    accel_bias_reg[1] -= (accel_bias[1] / 8);
    accel_bias_reg[2] -= (accel_bias[2] / 8);
 
    tmp_data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
    tmp_data[1] = (accel_bias_reg[0]) & 0xFF;
    tmp_data[1] = tmp_data[1] | mask_bit[0];
    tmp_data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
    tmp_data[3] = (accel_bias_reg[1]) & 0xFF;
    tmp_data[3] = tmp_data[3] | mask_bit[1];
    tmp_data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
    tmp_data[5] = (accel_bias_reg[2]) & 0xFF;
    tmp_data[5] = tmp_data[5] | mask_bit[2];
 
    // Push accelerometer biases to hardware registers:
    SetXAccelOffset(((int16_t) tmp_data[0]) << 8 | tmp_data[1]);
    SetYAccelOffset(((int16_t) tmp_data[2]) << 8 | tmp_data[3]);
    SetZAccelOffset(((int16_t) tmp_data[4]) << 8 | tmp_data[5]);
 
    // Output scaled accelerometer biases for subtraction in the main program:
    accel_bias_res[0] = (float) accel_bias[0] / (float) accel_sensitivity; 
    accel_bias_res[1] = (float) accel_bias[1] / (float) accel_sensitivity;
    accel_bias_res[2] = (float) accel_bias[2] / (float) accel_sensitivity;
}

void CMpu6050::SelfTest(float *destination)
{
    uint8_t self_test[6];
    float factory_trim[6];

    // Configure the accelerometer for self-test:
    SetAccelXSelfTest(true);
    SetAccelYSelfTest(true);
    SetAccelZSelfTest(true);
    SetFullScaleAccelRange(ACCEL_FULL_SCALE_RANGE_8);
    SetFullScaleGyroRange(GYRO_FULL_SCALE_RANGE_250);

    self_test[0] = GetAccelXSelfTestFactoryTrim();
    self_test[1] = GetAccelYSelfTestFactoryTrim();
    self_test[2] = GetAccelZSelfTestFactoryTrim();
    self_test[3] = GetGyroXSelfTestFactoryTrim();
    self_test[4] = GetGyroYSelfTestFactoryTrim();
    self_test[5] = GetGyroZSelfTestFactoryTrim();

    // Process results to allow final comparison with factory set values:
    factory_trim[0] = (4096.0f * 0.34f) * (pow((0.92f / 0.34f), ((self_test[0] - 1.0f) / 30.0f)));
    factory_trim[1] = (4096.0f * 0.34f) * (pow((0.92f / 0.34f), ((self_test[1] - 1.0f) / 30.0f)));
    factory_trim[2] = (4096.0f * 0.34f) * (pow((0.92f / 0.34f), ((self_test[2] - 1.0f) / 30.0f)));
    factory_trim[3] = ( 25.0f * 131.0f) * (pow(1.046f, (self_test[3] - 1.0f)));
    factory_trim[4] = (-25.0f * 131.0f) * (pow(1.046f, (self_test[4] - 1.0f)));
    factory_trim[5] = ( 25.0f * 131.0f) * (pow(1.046f, (self_test[5] - 1.0f)));
   
    // Report results as a ratio of "(STR - FT) / FT" (The change from Factory
    // Trim of the Self-Test Response).
    // To get to percent, must multiply by 100 and subtract result from 100.
    for (int i = 0; i < 6; i++)
        destination[i] = 100.0f + 100.0f * (self_test[i] - factory_trim[i]) / factory_trim[i];
}

void MadgwickQuaternionUpdate
(
    float accel_x,
    float accel_y,
    float accel_z,
    float gyro_x,
    float gyro_y,
    float gyro_z
)
{
    float func_1, func_2, func_3;
    float j_11o24, j_12o23, j_13o22, j_14o21, j_32, j_33;
    float q_dot_1, q_dot_2, q_dot_3, q_dot_4;
    float hat_dot_1, hat_dot_2, hat_dot_3, hat_dot_4;
    float gyro_x_err, gyro_y_err, gyro_z_err;
    float gyro_x_bias, gyro_y_bias, gyro_z_bias;
    float norm;
 
    float half_q1 = 0.5f * quart[0];
    float half_q2 = 0.5f * quart[1];
    float half_q3 = 0.5f * quart[2];
    float half_q4 = 0.5f * quart[3];
    float double_q1 = 2.0f * quart[0];
    float double_q2 = 2.0f * quart[1];
    float double_q3 = 2.0f * quart[2];
    float double_q4 = 2.0f * quart[3];
 
    // Normalise accelerometer measurement:
    norm = sqrt(accel_x * accel_x + accel_y * accel_y + accel_z * accel_z);
    
    // Handle NaN:
    if (norm == 0.0f)
        return;
    
    norm = 1.0f / norm;
    accel_x *= norm;
    accel_y *= norm;
    accel_z *= norm;
    
    // Compute the objective function and Jacobian:
    func_1 = double_q2 * quart[3] - double_q1 * quart[2] - accel_x;
    func_2 = double_q1 * quart[1] + double_q3 * quart[3] - accel_y;
    func_3 = 1.0f - double_q2 * quart[3] - double_q3 * quart[2] - accel_z;
    j_11o24 = double_q3;
    j_12o23 = double_q4;
    j_13o22 = double_q1;
    j_14o21 = double_q2;
    j_32 = 2.0f * j_14o21;
    j_33 = 2.0f * j_11o24;
          
    // Compute the gradient (matrix multiplication):
    hat_dot_1 = j_14o21 * func_2 - j_11o24 * func_1;
    hat_dot_2 = j_12o23 * func_1 + j_13o22 * func_2 - j_32 * func_3;
    hat_dot_3 = j_12o23 * func_2 - j_33 *func_3 - j_13o22 * func_1;
    hat_dot_4 = j_14o21 * func_1 + j_11o24 * func_2;
            
    // Normalize the gradient:
    norm = sqrt(hat_dot_1 * hat_dot_1 + hat_dot_2 * hat_dot_2 + hat_dot_3 * hat_dot_3 + hat_dot_4 * hat_dot_4);
    hat_dot_1 /= norm;
    hat_dot_2 /= norm;
    hat_dot_3 /= norm;
    hat_dot_4 /= norm;
            
    // Compute estimated gyroscope biases:
    gyro_x_err = double_q1 * hat_dot_2 - double_q2 * hat_dot_1 - double_q3 * hat_dot_4 + double_q4 * hat_dot_3;
    gyro_y_err = double_q1 * hat_dot_3 + double_q2 * hat_dot_4 - double_q3 * hat_dot_1 - double_q4 * hat_dot_2;
    gyro_z_err = double_q1 * hat_dot_4 - double_q2 * hat_dot_3 + double_q3 * hat_dot_2 - double_q4 * hat_dot_1;
            
    // Compute and remove gyroscope biases:
    gyro_x_bias += gyro_x_err * delta_t * ZETA;
    gyro_y_bias += gyro_y_err * delta_t * ZETA;
    gyro_z_bias += gyro_z_err * delta_t * ZETA;
            
    // Compute the quaternion derivative:
    q_dot_1 = -half_q2 * gyro_x - half_q3 * gyro_y - half_q4 * gyro_z;
    q_dot_2 =  half_q1 * gyro_x + half_q3 * gyro_z - half_q4 * gyro_y;
    q_dot_3 =  half_q1 * gyro_y - half_q2 * gyro_z + half_q4 * gyro_x;
    q_dot_4 =  half_q1 * gyro_z + half_q2 * gyro_y - half_q3 * gyro_x;
 
    // Compute then integrate estimated quaternion derivative:
    quart[0] += (q_dot_1 -(BETA * hat_dot_1)) * delta_t;
    quart[1] += (q_dot_2 -(BETA * hat_dot_2)) * delta_t;
    quart[2] += (q_dot_3 -(BETA * hat_dot_3)) * delta_t;
    quart[3] += (q_dot_4 -(BETA * hat_dot_4)) * delta_t;
 
    // Normalize the quaternion:
    norm = sqrt(quart[0] * quart[0] + quart[1] * quart[1] + quart[2] * quart[2] + quart[3] * quart[3]);
    norm = 1.0f / norm;
    quart[0] *= norm;
    quart[1] *= norm;
    quart[2] *= norm;
    quart[3] *= norm;
}