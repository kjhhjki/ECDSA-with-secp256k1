#include "int256.hpp"

namespace _int256
{
    __int256 __gcd(__int256 a, __int256 b)
    {
        if(a < __int256(0)) a = -a;
        if(b < __int256(0)) b = -b;
        if(a < b) std::swap(a, b);
        if (b == __int256(0))
            return a;
        return __gcd(b, a % b);
    }
}