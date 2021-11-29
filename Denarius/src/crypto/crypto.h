#ifndef DENARIUS_CRYPTO
#define DENARIUS_CRYPTO

#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>

namespace dCrypto {
	std::string SHA3512HashString(std::string str);
}

#endif //!DENARIUS_CRYPTO