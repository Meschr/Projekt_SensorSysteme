set(SOURCEFILES "esp32_i2c_rw.cpp")

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS ".")