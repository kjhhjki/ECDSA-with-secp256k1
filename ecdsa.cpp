#include "ecdsa.hpp"
#include "secret_gen.hpp"
#include "sha256.hpp"

#include <bits/stdc++.h>

using namespace std;

namespace ecdsa
{
    using i32 = int;
    using i64 = long long;
    using u32 = unsigned int;
    using u64 = unsigned long long;

    std::pair<_int256::__int256, _int256::__int256> sign(const std::string &message, const _int256::__int256 &secret_key)
    {
        _int256::__int256 k, hsh = Ecc::transfer(SHA256(message));
        Ecc::Point p;
        _int256::__int256 r = 0; // p.x % Ecc::N;
        _int256::__int256 s = 0; // ((_int256::__int256(SHA256(message)) + r * secret_key) * Ecc::inv(k)) % Ecc::N;
        while(s == _int256::__int256(0) || r == _int256::__int256(0))
        {
            k = Ecc::get_secret_key();
            p = Ecc::G * k;
            r = p.x % Ecc::N;
            s = (hsh + r * secret_key) % Ecc::N * Ecc::inv(k, Ecc::N) % Ecc::N;
        }
        // cout << hsh << '\n';
        // cout << r << ' ' << s << endl;
        // cout << Ecc::N << endl;
        return std::pair<_int256::__int256, _int256::__int256>(r, s);
    }

    bool verify(const std::string &message, const std::pair<_int256::__int256,_int256::__int256> &signature, const Ecc::Point &public_key)
    {
        auto [r, s] = signature;
        if(r < _int256::__int256(1) || r >= _int256::__int256(Ecc::N) || s < _int256::__int256(1) || s >= _int256::__int256(Ecc::N)) return false; 
        _int256::__int256 hsh = Ecc::transfer(SHA256(message));
        // cout << hsh << '\n';
        _int256::__int256 w = Ecc::inv(s, Ecc::N) % Ecc::N;
        _int256::__int256 u1 = (hsh * w) % Ecc::N;
        _int256::__int256 u2 = (r * w) % Ecc::N;
        Ecc::Point p = Ecc::G * u1 + public_key * u2;
        if(p.x == _int256::__int256(0) && p.y == _int256::__int256(0)) return false;
        // cout << p.x % Ecc::N << ' ' << r << endl;
        // cout << p.x % Ecc::N - r << endl;
        return ((p.x % Ecc::N) + Ecc::N) % Ecc::N == r;
    }
}