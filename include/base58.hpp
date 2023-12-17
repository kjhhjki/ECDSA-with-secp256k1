#ifndef _BASE58_HPP_
#define _BASE58_HPP_

#include <string>
#include <vector>
#include <stdint.h>

std::string base58_encode(const std::vector<uint8_t> &s);
std::string base58_encode(const std::string &s);

std::string base58_decode(const std::vector<uint8_t> &s);
std::string base58_decode(const std::string &s);

bool is_base58(const std::string &s);

std::string base58check_encode(const std::vector<uint8_t> &s);
std::string base58check_encode(const std::string &s);

std::string base58check_decode(const std::vector<uint8_t> &s);
std::string base58check_decode(const std::string &s);

bool is_base58check(const std::string &s);

#endif