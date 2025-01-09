cmake_minimum_required(VERSION 3.14)
project(ServerDevice.ethernet)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    if(CMAKE_CL_64)
        link_directories(tools/varmanager/win64 tools/byte_decoder/win64 tools/serverlogger/win64 tools/watchdog/win64)
    else()
        link_directories(tools/varmanager/win32 tools/byte_decoder/win32 tools/serverlogger/win32 tools/watchdog/win32)
    endif()

elseif(UNIX AND NOT APPLE)
    link_directories(tools/varmanager/linux tools/byte_decoder/linux tools/serverlogger/linux tools/watchdog/linux)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

include_directories(src tools)

add_subdirectory(test)

add_library(MTestServer.Device.EtherNetIP SHARED mtestserver_device_etherNetIP.cpp)

if(WIN32)
    target_link_libraries(MTestServer.Device.EtherNetIP PUBLIC varmanager bytedecoder serverlogger MTestSerVer.WatchDog Ws2_32 winmm)
else ()
    target_link_libraries(MTestServer.Device.EtherNetIP PUBLIC varmanager bytedecoder serverlogger MTestSerVer.WatchDog)
endif()

add_executable(Test0 test.cpp)
target_link_libraries(Test0 PUBLIC varmanager bytedecoder serverlogger MTestSerVer.WatchDog Ws2_32 winmm)

add_executable(Test1 test1.cpp)
target_link_libraries(Test1 PUBLIC varmanager bytedecoder serverlogger MTestSerVer.WatchDog)