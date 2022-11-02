set(SOURCEFILES "DataLogStateMachine.cpp")

idf_component_register(SRCS ${SOURCEFILES}
                        INCLUDE_DIRS "."
                        REQUIRES FileStorage Drivers)