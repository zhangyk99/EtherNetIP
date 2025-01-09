cmake_minimum_required(VERSION 3.14)

project(TestProject)

set(VCPKG ON)  # 或者根据需要设置为 ON


if (VCPKG)
    find_package(GTest CONFIG REQUIRED)
else ()
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://gitee.com/mirrors/googletest.git
            GIT_TAG v1.13.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()

include(GoogleTest)

add_executable(EIPTest EIPtest.cpp)
target_include_directories(EIPTest PRIVATE ../src ../tools gtest)

if (WIN32)
    target_link_libraries(EIPTest PUBLIC GTest::gtest GTest::gtest_main PUBLIC varmanager bytedecoder serverlogger MTestServer.WatchDog Ws2_32 winmm)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(EIPTest PUBLIC GTest::gtest GTest::gtest_main PUBLIC varmanager bytedecoder serverlogger MTestServer.WatchDog dl)
endif ()

gtest_discover_tests(EIPTest)