#include "secret_gen.hpp"
#include <chrono>
#include <random>

using namespace std;

using u32 = unsigned int;
using i32 = int;
using i64 = long long;
using u64 = unsigned long long;

constexpr u32 N = -1;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<u32> dis(0, N);

u64 mask = std::chrono::system_clock::now().time_since_epoch().count();

u64 xor_shift(u64 x)
{
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

std::vector<uint32_t> secret_key_gen(uint32_t n)
{
    std::vector<u32> res(n);
    for(u32 i = 0; i < n; ++i)
        res[i] = xor_shift(dis(gen));
    return res;
}
uint32_t irandom()
{
    return dis(gen);
}