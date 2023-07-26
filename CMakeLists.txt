cmake_minimum_required(VERSION 3.22.1)
project(keyser)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(keyser ${SOURCES})

target_link_libraries(${PROJECT_NAME} ssl crypto)