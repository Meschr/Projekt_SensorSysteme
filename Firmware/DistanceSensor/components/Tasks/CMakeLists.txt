set(SOURCEFILES "tFast.cpp" "tLog.cpp" )

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES Logic Gpio)