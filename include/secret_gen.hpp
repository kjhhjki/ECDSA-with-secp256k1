#ifndef SECRET_KEY_GEN_HPP
#define SECRET_KEY_GEN_HPP

#include <vector>
#include <stdint.h>

std::vector<uint32_t> secret_key_gen(uint32_t n);
uint32_t irandom();

#endif