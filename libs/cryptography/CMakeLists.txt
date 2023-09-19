add_library(cryptography cryptography.hpp cryptography.cpp)

target_link_libraries(cryptography ssl crypto)