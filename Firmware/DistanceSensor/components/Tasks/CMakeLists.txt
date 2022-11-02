set(SOURCEFILES "tFast.cpp" )

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES Logic)