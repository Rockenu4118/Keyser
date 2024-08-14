add_library(cryptography crypto.cpp ec.cpp sha.cpp Stealth.cpp utils.cpp)

target_link_libraries(cryptography ssl crypto)