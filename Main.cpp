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

    string secret_key_str;
    // = Ecc::get_secret_key();
    cout << "Please input the secret key: " << endl;
    cin >> secret_key_str;
    const _int256::__int256 SecretKey(secret_key_str, 16);
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
    // cout << Ecc::to_string(r, 16) << endl;
    // cout << Ecc::to_string(s, 16) << endl;
    cout << "Signature r: " << base58check_encode(Ecc::to_string(r, 16)) << endl;
    cout << "Signature s: " << base58check_encode(Ecc::to_string(s, 16)) << endl;
    
    cout << "Sign Complete." << endl;

    string verify_public_key, verify_sign_s, verify_sign_r;

    cout << "Please input the public key(compressed in base58): " << endl;
    cin >> verify_public_key;
    cout << "Please input the sign r: " << endl;
    cin >> verify_sign_r;
    cout << "Please input the sign s: " << endl;
    cin >> verify_sign_s;
    verify_public_key = base58check_decode(verify_public_key);
    verify_sign_r = base58check_decode(verify_sign_r);
    verify_sign_s = base58check_decode(verify_sign_s);

    // cout << verify_sign_r << endl << verify_sign_s << endl;

    auto verify_public_key_point = Ecc::decompress_key(verify_public_key);
    auto verify_sign = pair<_int256::__int256, _int256::__int256>(_int256::__int256(verify_sign_r, 16), _int256::__int256(verify_sign_s, 16));

    // cout << Ecc::original_key(PublicKey) << endl;
    // cout << Ecc::original_key(verify_public_key_point) << endl;

    if(ecdsa::verify(message, verify_sign, verify_public_key_point)) 
        cout << "Verified!" << endl;
    else 
        cout << "Failed!" << endl;

    return 0;

    return 0;
}