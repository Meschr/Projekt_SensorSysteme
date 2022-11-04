set(SOURCEFILES "Ls7366R.cpp" "mpu6050.cpp")

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES Spi I2c)