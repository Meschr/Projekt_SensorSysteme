set(SOURCEFILES "Sdmmc.cpp")

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES fatfs vfs sdmmc)