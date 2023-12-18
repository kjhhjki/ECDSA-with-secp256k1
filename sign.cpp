#include <bits/stdc++.h>

#include "ecc.hpp"
#include "sha256.hpp"
#include "base58.hpp"
#include "ecdsa.hpp"

#define For(i,a,b) for(i32 i = (a), endi = (b); i <= endi; ++i)
#define foR(i,a,b) for(i32 i = (a), endi = (b); i >= endi; --i)
#define ForE(i,u)  for(i32 i = head[u]; i; i = e[i].nxt)

using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using pii = pair<i32,i32>;

int main()
{

    cout << "Basic Attributes:" << endl;
    cout << "P: " << Ecc::to_string(Ecc::P, 16) << endl;
    cout << "N: " << Ecc::to_string(Ecc::N, 16) << endl;
    cout << "G: " << "04 " << Ecc::to_string(Ecc::G.x, 16) << ' ' << Ecc::to_string(Ecc::G.y, 16) << endl;
    cout << "H: " << Ecc::to_string(Ecc::H, 16) << endl;

    const _int256::__int256 SecretKey = Ecc::get_secret_key();
    cout << "SecretKey: " << Ecc::to_string(SecretKey, 16) << endl;

    const Ecc::Point PublicKey = Ecc::get_public_key(SecretKey);
    
    const string PublicKey_str = base58check_encode(Ecc::compress_key(PublicKey));
    cout << "PublicKey: \n" ;
    cout << "    Compressed : " << Ecc::compress_key(PublicKey) << endl;
    cout << "    Base58     : " << PublicKey_str << endl;
    
    string message;
    cout << "Please input the message:" << endl;
    cin >> message;
    auto Signature = ecdsa::sign(message, SecretKey);
    auto [r, s] = Signature;
    cout << "Signature r: " << base58check_encode(Ecc::to_string(r, 16)) << endl;
    cout << "Signature s: " << base58check_encode(Ecc::to_string(s, 16)) << endl;
    
    cout << "Sign Complete." << endl;

    return 0;
}