#include "base58.hpp"
#include "sha256.hpp"
#include <algorithm>
#include <iostream>
using namespace std;

const string p = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
const int base58_map[128] = 
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, -1,
    -1, -1, -1, 9, 10, 11, 12, 13, 14, 15, 16, -1,
    17, 18, 19, 20, 21, -1, 22, 23, 24, 25, 26, 27, 28, 
    29, 30, 31, 32, -1, -1, -1, -1, -1, -1, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
};

string to_string(uint32_t x, uint32_t base)
{
    string res;
    while(x)
    {
        res += "0123456789abcdef"[x % base];
        x /= base;
    }
    reverse(res.begin(), res.end());
    return res;
}

string base58_encode(const vector<uint8_t> &s)
{
    constexpr int base = 58;
    string res;
    uint32_t x = 0;
    for(auto c : s)
    {
        x = x * 256 + c;
        if(x < base) continue;
        while(x > base)
        {
            res += p[x % base];
            x /= base;
        }
    }
    while(x)
    {
        res += p[x % base];
        x /= base;
    }
    reverse(res.begin(), res.end());
    return res;
}
string base58_encode(const string &s)
{
    constexpr int base = 58;
    string res;
    uint32_t x = 0;
    for(auto c : s)
    {
        x = x * 256 + c;
        if(x < base) continue;
        while(x > base)
        {
            res += p[x % base];
            x /= base;
        }
    }
    while(x)
    {
        res += p[x % base];
        x /= base;
    }
    reverse(res.begin(), res.end());
    return res;
}

string base58_decode(const vector<uint8_t> &s)
{
    constexpr int base = 256;
    string res;
    uint32_t x = 0;
    for(auto c : s)
    {
        int ch = base58_map[c];
        if(ch == -1) return "";
        x = x * 58 + ch;
        if(x < base) continue;
        while(x > base)
        {
            res += x % base;
            x /= base;
        }
    }
    while(x)
    {
        res += x % base;
        x /= base;
    }
    reverse(res.begin(), res.end());
    return res;
}
string base58_decode(const string &s)
{
    constexpr int base = 256;
    string res;
    uint32_t x = 0;
    for(auto c : s)
    {
        int ch = base58_map[c];
        if(ch == -1) return "";
        x = x * 58 + ch;
        if(x < base) continue;
        while(x > base)
        {
            res += x % base;
            x /= base;
        }
    }
    while(x)
    {
        res += x % base;
        x /= base;
    }
    reverse(res.begin(), res.end());
    return res;
}

bool is_base58(const string &s)
{
    string tmp = base58_decode(s);
    return tmp != "";
}


string base58check_encode(const vector<uint8_t> &s)
{
    string chk = to_string(SHA256(SHA256(s))[0], 16);
    while(chk.size() < 8) chk = "0" + chk;
    string res = base58_encode(string(s.begin(),s.end())+chk);
    return res;
}
string base58check_encode(const string &s)
{
    string chk = to_string(SHA256(SHA256(s))[0], 16);
    while(chk.size() < 8) chk = "0" + chk;
    string res = base58_encode(s+chk);
    return res;
}

string base58check_decode(const vector<uint8_t> &s)
{
    string res = base58_decode(s);
    if(res.size() < 8) return "";
    string chk = res.substr(res.size()-8);
    res = res.substr(0, res.size()-8);
    string chk2 = to_string(SHA256(SHA256(res))[0], 16);
    while(chk2.size() < 8) chk2 = "0" + chk2;
    if(chk != chk2) return "";
    return res;
}
string base58check_decode(const string &s)
{
    string res = base58_decode(s);
    if(res.size() < 8) return "";
    string chk = res.substr(res.size()-8);
    res = res.substr(0, res.size()-8);
    string chk2 = to_string(SHA256(SHA256(res))[0], 16);
    while(chk2.size() < 8) chk2 = "0" + chk2;
    if(chk != chk2) return "";
    return res;
}

bool is_base58check(const string &s)
{
    string tmp = base58check_decode(s);
    return tmp != "";
}