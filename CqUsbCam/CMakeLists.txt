
project(CqUsbCamAPI)

cmake_minimum_required(VERSION 2.8)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -g ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11 -O3 -Wall -g")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(sensors)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./sensors SENSORS_DIR_SRCS)

LINK_DIRECTORIES("/home/nxb/Desktop/USBCamera/USB30/FX3_SDK_Linux/FX3_SDK_Linux_v1.3.3/cyusb_linux_1.0.4/lib")
link_libraries(cyusb)

add_library(CqUsbCam SHARED ${DIR_SRCS} ${SENSORS_DIR_SRCS})

target_link_libraries (CqUsbCam  cyusb)


