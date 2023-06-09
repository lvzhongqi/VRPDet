cmake_minimum_required(VERSION 3.10.2)
#项目名称
project("jninative")

add_definitions(-std=c++11)

set(${PROJECT_NAME}_SOURCES
        jninative.cpp
        )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI})

add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log)