cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME keycrypto)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC aes.cpp crypto.cpp ec.cpp sha.cpp Stealth.cpp utils.cpp)

target_link_libraries(${PROJECT_NAME} ssl crypto)