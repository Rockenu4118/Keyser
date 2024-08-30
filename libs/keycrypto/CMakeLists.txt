cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME keycrypto)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "keycrypto/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} ssl crypto)