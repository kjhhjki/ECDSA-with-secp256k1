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
    string verify_public_key, verify_sign_s, verify_sign_r;

    string verify_message;
    cout << "Please input the message:" << endl;
    cin >> verify_message;
    cout << "Please input the public key(compressed in base58): " << endl;
    cin >> verify_public_key;
    cout << "Please input the sign r: " << endl;
    cin >> verify_sign_r;
    cout << "Please input the sign s: " << endl;
    cin >> verify_sign_s;
    verify_public_key = base58check_decode(verify_public_key);
    verify_sign_r = base58check_decode(verify_sign_r);
    verify_sign_s = base58check_decode(verify_sign_s);

    auto verify_public_key_point = Ecc::decompress_key(verify_public_key);
    auto verify_sign = pair<_int256::__int256, _int256::__int256>(_int256::__int256(verify_sign_r, 16), _int256::__int256(verify_sign_s, 16));

    if(ecdsa::verify(verify_message, verify_sign, verify_public_key_point)) 
        cout << "Verified!" << endl;
    else 
        cout << "Failed!" << endl;

    return 0;
}