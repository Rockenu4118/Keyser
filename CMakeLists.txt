cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME "Keyser")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp"
                  "src/app/*.cpp"
                  "src/utils/*.cpp"
                  "src/views/*.cpp"
                  "src/chain/*.cpp"
                  "src/net/*.cpp"
                  "src/node/*.cpp"
                  "src/rpc/*.cpp"
                  "src/storage/*.cpp"
#                  "src/validation/*.cpp"
                  "src/wallet/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

#add_subdirectory(libs/keycrypto)

#target_link_libraries(${PROJECT_NAME} ssl crypto leveldb snappy keycrypto cryptography net_core cli_core)
target_link_libraries(${PROJECT_NAME} ssl crypto ncurses)


target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}" 
                                                   "${PROJECT_SOURCE_DIR}/libs/keycrypto")