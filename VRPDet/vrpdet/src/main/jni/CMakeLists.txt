cmake_minimum_required(VERSION 3.10.2)
#项目名称
project("jninative")

add_definitions(-std=c++11)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(${PROJECT_NAME}_SOURCES
        jninative.cpp
        )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI})

add_library(crypto_1_1 SHARED IMPORTED)

set_target_properties(crypto_1_1
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_1_1.so)
add_library(ssl_1_1 SHARED IMPORTED)

set_target_properties(ssl_1_1
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libssl_1_1.so)
add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log
        ssl_1_1
        crypto_1_1)
