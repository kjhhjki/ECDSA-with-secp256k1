#ifndef _ECDSA_HPP_
#define _ECDSA_HPP_

#include "ecc.hpp"

namespace ecdsa
{
    std::pair<_int256::__int256,_int256::__int256> sign(const std::string &message, const _int256::__int256 &secret_key);
    bool verify(const std::string &message, const std::pair<_int256::__int256,_int256::__int256> &signature, const Ecc::Point &public_key);
}

#endif