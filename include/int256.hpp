#ifndef _INT256_HPP_
#define _INT256_HPP_

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

namespace _int256
{
    typedef int i32;
    typedef long long i64;
    typedef unsigned long long u64;

    constexpr i32 mp[128] = 
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,
        0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0
    };

    constexpr static i32 N = 237, base = 1e6;
    class __int256
    {
    public:
        i32 a[N], len, neg;
        void write(i32 _a) const
        {
            if (_a > 9)
                write(_a / 10);
            putchar((_a % 10) | 48);
        }
        i32 getlen(i32 _a) const
        {
            i32 ans = 0;
            while (_a)
                _a /= 10, ++ans;
            return ans;
        }

    public:
        __int256() { len = 1, neg = 0, memset(a, 0, sizeof(a)); }
        void init(std::string s, i32 radix = 10)
        {
            i32 lens = s.length(); len = 0;
            std::fill(a, a + N, 0);
            if(lens <= 0) return;
            if(s[0] == '-') neg = 1, s = s.substr(1, lens-1), lens--;
            else neg = 0;
            if(lens <= 0)  return;
            std::vector<i32> tmp(lens, 0);
            for(i32 i = 1; i <= lens; ++i) tmp[i-1] = mp[s[lens-i]];
            
            while(lens)
            {
                for (int i = lens-1; i > 0; --i)
                {
                    tmp[i-1] += tmp[i]%base*radix;
                    tmp[i] = tmp[i]/base;
                }
                a[++len] = tmp[0]%base;
                tmp[0] /= base;
                while(lens>0&&!tmp[lens-1]) lens--;
            }
            // std::cout << "Here\n";
        }
        __int256(char *s, i32 radix = 10)
        {
            std::string t(s);
            init(t, radix);
        }
        __int256(std::string s, i32 radix = 10)
        {
            init(s, radix);
        }
        __int256(const char *s, i32 radix = 10)
        {
            std::string t(s);
            init(t, radix);
        }
        __int256(const __int256 &x)
        {
            len = x.len;
            neg = x.neg;
            std::copy(x.a, x.a + N, a);
        }
        template <typename _Tp>
        __int256(_Tp x)
        {
            len = 0;
            neg = 0;
            memset(a, 0, sizeof(a));
            if (x < 0)
                x = -x, neg = 1;
            i32 tmp;
            while ((tmp = x / base))
            {
                a[++len] = x % base;
                x = tmp;
            }
            a[++len] = x;
        }
        void get()
        {
            std::string s = std::string();
            char ch = getchar();
            while (ch < '0' || ch > '9')
            {
                if (ch == '-' && !s.size())
                    s += '-';
                ch = getchar();
            }
            while (ch >= '0' && ch <= '9')
                s += ch, ch = getchar();
            *this = __int256(s);
        }
        void print() const
        {
            i32 baselen = getlen(base) - 1;
            if (neg)
                putchar('-');
            write(a[len]);
            for (i32 i = len - 1, tmp; i; --i)
            {
                tmp = baselen - getlen(a[i]);
                while (tmp--)
                    putchar('0');
                if (a[i])
                    write(a[i]);
            }
        }
        std::string to_string()
        {
            std::string res = "";
            i32 baselen = getlen(base) - 1;
            if (neg)
                res += "-";
            std::string tmp1 = "";
            i32 tmp2 = a[len];
            if (!a[len])
                return "0";
            while (tmp2)
                tmp1 += tmp2 % 10 + '0', tmp2 /= 10;
            std::reverse(tmp1.begin(), tmp1.end());
            res += tmp1;
            for (i32 i = len - 1, tmp; i; --i)
            {
                tmp = baselen - getlen(a[i]);
                while (tmp--)
                    res += "0";
                if (!a[i])
                    continue;
                tmp1 = "";
                tmp2 = a[i];
                while (tmp2)
                    tmp1 += tmp2 % 10 + '0', tmp2 /= 10;
                std::reverse(tmp1.begin(), tmp1.end());
                res += tmp1;
            }
            return res;
        }
        friend std::istream &operator>>(std::istream &input, __int256 &x)
        {
            x.get();
            return input;
        }
        friend std::ostream &operator<<(std::ostream &output, const __int256 &x)
        {
            x.print();
            return output;
        }
        bool operator==(const __int256 &x) const
        {
            if (len != x.len || neg != x.neg)
                return false;
            for (i32 i = 1; i <= len; ++i)
                if (a[i] != x.a[i])
                    return false;
            return true;
        }
        bool operator!=(const __int256 &x) const { return !(*this == x); }
        bool operator>(const __int256 &x) const
        {
            if (neg != x.neg)
                return x.neg;
            if (len != x.len)
                return len > x.len;
            for (i32 i = len; i; --i)
                if (a[i] != x.a[i])
                    return a[i] > x.a[i];
            return false;
        }
        bool operator<(const __int256 &x) const
        {
            if (neg != x.neg)
                return neg;
            if (len != x.len)
                return len < x.len;
            for (i32 i = len; i; --i)
                if (a[i] != x.a[i])
                    return a[i] < x.a[i];
            return false;
        }
        bool operator>=(const __int256 &x) const { return !(*this < x); }
        bool operator<=(const __int256 &x) const { return !(*this > x); }

        __int256 operator-() const
        {
            __int256 x(*this);
            x.neg ^= 1;
            return x;
        }
        __int256 operator+(const __int256 &x) const
        {
            if ((!neg) && x.neg)
                return *this - (-x);
            if (neg && (!x.neg))
                return x - (-*this);
            __int256 ans = __int256();
            ans.len = std::max(len, x.len);
            for (i32 i = 1; i <= ans.len; ++i)
            {
                ans.a[i] += a[i] + x.a[i];
                if (ans.a[i] >= base)
                    ans.a[i] -= base, ++ans.a[i + 1];
            }
            if (ans.a[ans.len + 1])
                ++ans.len;
            if (neg && x.neg)
                ans.neg = 1;
            return ans;
        }
        __int256 operator+=(const __int256 &x) { return *this = *this + x; }
        __int256 operator-(const __int256 &x) const
        {
            if ((!neg) && x.neg)
                return *this + (-x);
            if (neg && x.neg)
                return (-x) - (-*this);
            if (neg && (!x.neg))
                return -((-*this) + x);
            __int256 ans = __int256();
            if (*this == x)
                return ans;
            if (x > *this)
            {
                ans = (x - *this);
                ans.neg = 1;
                return ans;
            }
            ans.len = std::max(len, x.len);
            for (i32 i = 1; i <= ans.len; ++i)
            {
                ans.a[i] += a[i] - x.a[i];
                if (ans.a[i] < 0)
                    ans.a[i] += base, --ans.a[i + 1];
            }
            while (ans.len && !ans.a[ans.len])
                --ans.len;
            return ans;
        }
        __int256 operator-=(const __int256 &x) { return *this = *this - x; }
        __int256 operator*(const __int256 &x) const
        {

            __int256 ans = __int256();
            if (*this == ans || x == ans)
                return ans;
            if (neg != x.neg)
                ans.neg = 1;
            ans.len = len + x.len;
            u64 tmp;
            for (i32 i = 1; i <= len; ++i)
                for (i32 j = 1; j <= x.len; ++j)
                {
                    tmp = 1ull * a[i] * x.a[j] + ans.a[i + j - 1];
                    if (tmp >= base)
                    {
                        ans.a[i + j] += tmp / base;
                        ans.a[i + j - 1] = tmp % base;
                    }
                    else
                        ans.a[i + j - 1] = tmp;
                }
            while (!ans.a[ans.len])
                --ans.len;
            return ans;
        }
        __int256 operator*=(const __int256 &x) { return *this = *this * x; }
        __int256 operator/(const __int256 &X) const
        {
            if (X == __int256(0))
                std::cerr << "Error:divide 0\n", exit(-1);
            __int256 ans(*this), x(X), tmp(1), lt = __int256();
            if (neg != x.neg)
                ans.neg = 1;
            while (ans >= x)
                x *= 2, tmp *= 2;
            while (tmp.len > 1 || tmp.a[1])
            {
                if (ans >= x)
                    ans -= x, lt += tmp;
                x /= 2, tmp /= 2;
            }
            ans = lt;
            while (ans.len && !ans.a[ans.len])
                --ans.len;
            if (!ans.len)
                return __int256();
            return ans;
        }
        __int256 operator/=(const __int256 &X)
        {
            if (X == __int256(0))
                std::cerr << "Error:divide 0\n", exit(-1);
            __int256 x(X), tmp(1), lt = __int256();
            if (neg != x.neg)
                neg = 1;
            else
                neg = 0;
            while (*this >= x)
                x *= 2, tmp *= 2;
            while (tmp.len > 1 || tmp.a[1])
            {
                if (*this >= x)
                    *this -= x, lt += tmp;
                x /= 2, tmp /= 2;
            }
            *this = lt;
            while (len && !a[len])
                --len;
            if (!len)
                return *this = __int256();
            return *this;
        }
        __int256 operator%(const __int256 &X) const
        {
            if (X == __int256(0))
                std::cerr << "Error:mod 0\n", exit(-1);
            __int256 ans(*this), x(X), tmp(1), lt = __int256();
            if (neg != x.neg)
                ans.neg = 1;
            while (ans >= x)
                x *= 2, tmp *= 2;
            while (tmp.len > 1 || tmp.a[1])
            {
                if (ans >= x)
                    ans -= x, lt += tmp;
                x /= 2, tmp /= 2;
            }
            if(ans.neg) ans += X;
            return ans;
        }
        __int256 operator%=(const __int256 &X)
        {
            if (X == __int256(0))
                std::cerr << "Error:mod 0\n", exit(-1);
            __int256 x(X), tmp(1), lt = __int256();
            if (neg != x.neg)
                neg = 1;
            else
                neg = 0;
            while (*this >= x)
                x *= 2, tmp *= 2;
            while (tmp.len > 1 || tmp.a[1])
            {
                if (*this >= x)
                    *this -= x, lt += tmp;
                x /= 2, tmp /= 2;
            }
            if(neg) *this += X;
            return *this;
        }

        template <typename _Tp>
        operator _Tp() const { return _Tp(a[1]); }
        template <typename _Tp>
        __int256 operator+(const _Tp &x) const { return *this + __int256(x); }
        template <typename _Tp>
        __int256 operator+=(const _Tp &x) { return *this = *this + x; }
        template <typename _Tp>
        __int256 operator-(const _Tp &x) const { return *this - __int256(x); }
        template <typename _Tp>
        __int256 operator-=(const _Tp &x) { return *this = *this - x; }
        template <typename _Tp>
        __int256 operator*(const _Tp &x) const { return *this * __int256(x); }
        template <typename _Tp>
        __int256 operator*=(const _Tp &x) { return *this = *this * x; }
        template <typename _Tp>
        __int256 operator/(const _Tp &x) const
        {
            if (x == 0)
                std::cerr << "Error:divide 0\n", exit(-1);
            __int256 ans = __int256();
            if (len == 1 && x > a[1])
                return ans;
            u64 res = 0;
            ans.len = len;
            if (neg != (x < 0))
                ans.neg = 1;
            for (i32 i = len; i; --i)
            {
                res = res * base + a[i];
                ans.a[i] = res / x;
                res %= x;
            }
            while (ans.len > 1 && !ans.a[ans.len])
                --ans.len;
            return ans;
        }
        template <typename _Tp>
        __int256 operator/=(const _Tp &x) { return *this = *this / x; }
        template <typename _Tp>
        __int256 operator%(const _Tp &x) const
        {
            if (x == 0)
                std::cerr << "Error:mod 0\n", exit(-1);
            if (len == 1 && x > a[1])
                return *this;
            u64 res = 0;
            for (i32 i = len; i; --i)
            {
                res = res * base + a[i];
                res %= x;
            }
            return res;
        }
        template <typename _Tp>
        __int256 operator%=(const _Tp &x) { return *this = *this % x; }
    };
    template <typename _Tp>
    const __int256 operator+(const _Tp &x, const __int256 &y) { return __int256(x) + y; }
    template <typename _Tp>
    const __int256 operator-(const _Tp &x, const __int256 &y) { return __int256(x) - y; }
    template <typename _Tp>
    const __int256 operator*(const _Tp &x, const __int256 &y) { return __int256(x) * y; }
    template <typename _Tp>
    const __int256 operator/(const _Tp &x, const __int256 &y) { return __int256(x) / y; }
    template <typename _Tp>
    const __int256 operator%(const _Tp &x, const __int256 &y) { return __int256(x) % y; }

    __int256 __gcd(__int256 a, __int256 b);
}

#endif