#include "ecc.hpp"
#include "sha256.hpp"

using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

namespace Ecc
{
    const _int256::__int256 P ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);
    const Point G(
        _int256::__int256("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", 16), 
        _int256::__int256("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8", 16)
        );
    const _int256::__int256 N ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141", 16);
    const _int256::__int256 H ("1");

    constexpr char tar[] = "0123456789abcdef";

    _int256::__int256 exgcd(_int256::__int256 a, _int256::__int256 b, _int256::__int256 &x, _int256::__int256 &y)
    {
        if(b == _int256::__int256(0))
        {
            x = _int256::__int256(1);
            y = _int256::__int256(0);
            return a;
        }
        _int256::__int256 res = exgcd(b, a%b, x, y);
        _int256::__int256 tmp = x;
        x = y;
        y = tmp - (a/b)*y;
        return res;
    }
    _int256::__int256 inv(_int256::__int256 a, _int256::__int256 p)
    {
        _int256::__int256 x, y;
        a = (a%p+p)%p;
        exgcd(a, p, x, y);
        x = (x%p+p)%p;
        // if(x*a%P != _int256::__int256(1)) cout << "inv error: " << a << ' ' << x << ' ' << x*a%P << endl;
        return x;
    }
    
    std::string to_string(uint32_t x, uint32_t base)
    {
        string res;
        while(x)
        {
            res += tar[x % base];
            x /= base;
        }
        reverse(res.begin(), res.end());
        return res;
    }
    std::string to_string(std::vector<uint32_t> s, int base, int radix)
    {
        i32 lens = s.size();
        string a;
        
        while(lens > 0)
        {
            for (int i = lens-1; i > 0; --i)
            {
                s[i-1] += s[i]%base*radix;
                s[i] = s[i]/base;
            }
            a.push_back(tar[s[0]%base]);
            s[0] /= base;
            while(lens>0&&!s[lens-1]) lens--;
        }
        std::reverse(a.begin(), a.end());
        return a;
    }
    std::string to_string(const _int256::__int256 &a, uint32_t base)
    {
        vector<uint32_t> tmp(a.a+1,a.a+a.len+1);
        return to_string(tmp, base);
    }

    Point Point::operator+(const Point &rhs) const
    {
        _int256::__int256 dy,dx,k;
        // cout << to_string(x,16) << ' ' << to_string(y,16) << endl;
        // cout << to_string(rhs.x,16) << ' ' << to_string(rhs.y,16) << endl;
        if(*this == rhs)
        {
            dy = (3*x*x) %P;
            dx = (2*y) %P;
        }
        else
        {
            dy = ((rhs.y - y)%P + P) %P;
            dx = ((rhs.x - x)%P + P) %P;
        }
        
        k = dy * inv(dx) % P;
        _int256::__int256 x3 = ((k*k%P - x - rhs.x)%P + P)%P;
        _int256::__int256 y3 = (((x - x3)%P*k%P - y)%P + P)%P;

        // cout << to_string(x3,16) << ' ' << to_string(y3,16) << endl << endl;
        return Point(x3, y3);
    }
    Point Point::operator*(const _int256::__int256 &rhs) const
    {
        Point res, tmp = *this;
        _int256::__int256 n(rhs);
        const static _int256::__int256 zero(0);
        i32 flag = 0;
        while(n > zero)
        {
            // cout << n << endl;
            // cout << res.x << ' ' << res.y << '\n';
            // cout << tmp.x << ' ' << tmp.y << '\n' << '\n';
            // static i32 cnt = 0;
            // if(++cnt > 137) 
            // {
            //     cout << "error" << endl;
            //     exit(0);
            // }
            if(n%2) 
            {
                if(flag) res += tmp;
                else res = tmp, flag = 1;
            }
            tmp += tmp;
            n /= 2;
        }
        return res;
    }
    Point& Point::operator+=(const Point &rhs)
    {
        *this = *this + rhs;
        return *this;
    }
    Point& Point::operator*=(const _int256::__int256 &rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    bool Point::operator==(const Point &rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool Point::operator!=(const Point &rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    _int256::__int256 transfer(const vector<uint32_t> &tmp)
    {
        string s;
        for(u32 i = 0; i < tmp.size(); ++i)
        {
            for(i32 j = 3; j >= 0; --j)
            {
                s += tar[(tmp[i] >> (j*8)) & 255];
            }
        }
        return _int256::__int256(s, 16);
    }
    _int256::__int256 get_secret_key()
    {
        vector<u32> tmp = secret_key_gen(8);                
        _int256::__int256 res = transfer(tmp);
        while(res >= N) 
        {
            static int cnt = 0;
            res = transfer(secret_key_gen(8));
        }
        return res;
    }
    Point get_public_key(const _int256::__int256 &secret_key)
    {
        return G * secret_key;
    }
    string original_key(const Point &public_key)
    {
        string res = "04";
        res += to_string(public_key.x, 16);
        res += to_string(public_key.y, 16);
        return res;
    }
    string compress_key(const Point &public_key)
    {
        string res;
        if(public_key.y.a[0] & 1) res += "03";
        else res += "02";
        res += to_string(public_key.x, 16);
        return res;
    }

    _int256::__int256 qpow(_int256::__int256 x, _int256::__int256 n)
    {
        _int256::__int256 res = _int256::__int256(1);
        while(n > _int256::__int256(0))
        {
            if(n%2) res = res * x % P;
            x = x * x % P;
            n /= 2;
        }
        return res;
    }

    Point decompress_key(const string &public_key)
    {
        _int256::__int256 x = _int256::__int256(public_key.substr(2, public_key.size()-2), 16),
                          a = (x * x % P * x % P + _int256::__int256(7)) % P,
                          y = qpow(a, (P + 1) / 4);
        if(y%2 && public_key[1] == '3') y = P - y;
        if((!y%2) && public_key[1] == '2') y = P - y;
        return Point(x, y);
    }
}