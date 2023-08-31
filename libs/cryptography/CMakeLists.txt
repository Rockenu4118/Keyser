add_library(cryptography cryptography.cpp cryptography.hpp)

target_link_libraries(cryptography ssl crypto)