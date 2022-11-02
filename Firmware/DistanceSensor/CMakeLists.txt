# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly

#set cmake to cpp17 version (need minimum cmake version 3.8)
set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.8)

#pulls in the rest of the CMake functionality to configure the project, discover all the components, etc.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

#additional cmake directories
set(EXTRA_COMPONENT_DIRS components)

#project name: is used for the final binary output files of the app 
project(DistanceSensor)