#ifndef SHA256_HPP
#define SHA256_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <stdint.h>

extern std::vector<uint32_t> SHA256(const std::string &s);
extern std::vector<uint32_t> SHA256(const std::vector<uint8_t> &s);
extern std::vector<uint32_t> SHA256(const std::vector<uint32_t> &a);
extern std::vector<uint32_t> SHA256(const std::vector<uint64_t> &a);

#endif