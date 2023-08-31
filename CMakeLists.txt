cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME "Keyser")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(libs/cryptography)

target_link_libraries(${PROJECT_NAME} ssl crypto cryptography)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/libs/cryptography")