#ifndef _ECC_HPP_
#define _ECC_HPP_

#include "int256.hpp"
#include "secret_gen.hpp"

namespace Ecc
{

    struct Point
    {
        _int256::__int256 x, y;
        Point() {}
        Point(_int256::__int256 x, _int256::__int256 y) : x(x), y(y) {}
        Point operator+(const Point &rhs) const;
        Point operator*(const _int256::__int256 &rhs) const;
        Point& operator+=(const Point &rhs);
        Point& operator*=(const _int256::__int256 &rhs);
        bool operator==(const Point &rhs) const;
        bool operator!=(const Point &rhs) const;
    };

    extern const Point G;
    extern const _int256::__int256 P;
    extern const _int256::__int256 N;
    extern const _int256::__int256 H;

    _int256::__int256 inv(_int256::__int256 a, _int256::__int256 p = P);
    std::string to_string(uint32_t x, uint32_t base);
    std::string to_string(std::vector<uint32_t> s, int base = 10, int radix = 1000000);
    std::string to_string(const _int256::__int256 &a, uint32_t base);
    _int256::__int256 transfer(const std::vector<uint32_t> &a);

    _int256::__int256 get_secret_key();
    Point get_public_key(const _int256::__int256 &secret_key);
    std::string original_key(const Point &public_key);
    std::string compress_key(const Point &public_key);
    Point decompress_key(const std::string &public_key);
}

#endif