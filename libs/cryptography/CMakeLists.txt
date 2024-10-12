add_library(cryptography cryptography.cpp)

target_link_libraries(cryptography ssl crypto)