#include "sha256.hpp"

constexpr uint32_t Constants[] = 
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
constexpr uint32_t Hsh0[] = 
{
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

constexpr uint32_t RotR(uint32_t x, uint32_t n) { return (x >> n) | (x << (32-n)); }
constexpr uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)  { return (x & y) ^ ((~x) & z); }
constexpr uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
constexpr uint32_t Sig0(uint32_t x) { return RotR(x, 2) ^ RotR(x, 13) ^ RotR(x, 22); }
constexpr uint32_t Sig1(uint32_t x) { return RotR(x, 6) ^ RotR(x, 11) ^ RotR(x, 25); }
constexpr uint32_t sig0(uint32_t x) { return RotR(x, 7) ^ RotR(x, 18) ^ (x >> 3); }
constexpr uint32_t sig1(uint32_t x) { return RotR(x, 17) ^ RotR(x, 19) ^ (x >> 10); }


std::vector<uint32_t> SHA256(const std::vector<uint8_t> &s)
{
    // Variables
    std::vector<uint32_t> hsh(Hsh0, Hsh0+8);
    std::vector<uint32_t> W(64);

    std::size_t N = s.size();

    // Preprocessing
    uint64_t len = N;
    len *= 8;
    uint32_t k = (448-1-len+512) %512;
    uint8_t msg[(len+1+k+64)>>3];
    std::copy(s.begin(), s.end(), msg);

    // Supplementation
    msg[N] = 0x80; 
    for(uint32_t i = 1; i < k/8; ++i) msg[N+i] = 0x00;
    for(uint32_t i = 1; i <= 8; ++i) msg[N+k/8+i] = (len >> (64-8*i)) & 0xff;

    // Hashing
    uint32_t n = (len+1+k+64) >> 9;
    for(uint32_t i = 0; i < n; ++i)
    {
        for(uint32_t j = 0; j < 16; ++j)
        {
            W[j] = ((uint32_t)(msg[i*64+j*4]) << 24) | ((uint32_t)(msg[i*64+j*4+1]) << 16) | 
                    (uint32_t)((msg[i*64+j*4+2]) << 8) | (uint32_t)(msg[i*64+j*4+3]);
        }
        for(uint32_t j = 16; j < 64; ++j)
        {
            W[j] = sig1(W[j-2]) + W[j-7] + sig0(W[j-15]) + W[j-16];
        }
        std::vector<uint32_t> tmp(hsh.begin(), hsh.end());
        uint32_t T1, T2;
        for(uint32_t j = 0; j < 64; ++j)
        {
            T1 = tmp[7] + Sig1(tmp[4]) + Ch(tmp[4], tmp[5], tmp[6]) + Constants[j] + W[j];
            T2 = Sig0(tmp[0]) + Maj(tmp[0], tmp[1], tmp[2]);
            for(uint32_t k = 7; k > 0; --k) tmp[k] = tmp[k-1];
            tmp[4] += T1; tmp[0] = T1 + T2;
        }
        for(uint32_t j = 0; j < 8; ++j) hsh[j] += tmp[j];
    }
    return hsh;
}

std::vector<uint32_t> SHA256(const std::string &s)
{
    std::vector<uint8_t> tmp(s.begin(), s.end());
    for(uint32_t i = 0; i < s.size(); ++i) tmp[i] = s[i];
    return SHA256(tmp);
}


std::vector<uint32_t> SHA256(const std::vector<uint32_t> &a)
{
    std::vector<uint8_t> tmp(a.size()*4);
    for(uint32_t i = 0; i < a.size(); ++i)
    {
        tmp[i*4] = a[i] >> 24;
        tmp[i*4+1] = a[i] >> 16;
        tmp[i*4+2] = a[i] >> 8;
        tmp[i*4+3] = a[i];
    }
    return SHA256(tmp);
}

std::vector<uint32_t> SHA256(const std::vector<uint64_t> &a)
{
    std::vector<uint8_t> tmp(a.size()*8);
    for(uint32_t i = 0; i < a.size(); ++i)
    {
        tmp[i*8] = a[i] >> 56;
        tmp[i*8+1] = a[i] >> 48;
        tmp[i*8+2] = a[i] >> 40;
        tmp[i*8+3] = a[i] >> 32;
        tmp[i*8+4] = a[i] >> 24;
        tmp[i*8+5] = a[i] >> 16;
        tmp[i*8+6] = a[i] >> 8;
        tmp[i*8+7] = a[i];
    }
    return SHA256(tmp);
}
