cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME "Keyser")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp" 
                  "src/utils/*.cpp" 
                  "src/views/*.cpp" 
                  "src/chain/*.cpp" 
                  "src/net/*.cpp" 
                  "src/node/*.cpp" 
                  "src/storage/*.cpp"
                  "src/wallet/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(libs/cli_core)
add_subdirectory(libs/cryptography)
add_subdirectory(libs/net_core)

target_link_libraries(${PROJECT_NAME} ssl crypto cryptography net_core cli_core)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}" 
                                                   "${PROJECT_SOURCE_DIR}/libs/cryptography" 
                                                   "${PROJECT_SOURCE_DIR}/libs/net_core"
                                                   "${PROJECT_SOURCE_DIR}/libs/cli_core")