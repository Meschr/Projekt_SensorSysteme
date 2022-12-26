set(SOURCEFILES "SpiBusHandler.cpp")

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES driver)