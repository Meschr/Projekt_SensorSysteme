set(SOURCEFILES "GpioInit.cpp")

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES Logic)