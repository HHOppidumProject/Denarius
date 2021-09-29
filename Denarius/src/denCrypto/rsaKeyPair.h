#ifndef DENARIUS_RSA_KEY_PAIR
#define DENARIUS_RSA_KEY_PAIR

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

namespace dCrypto {
	class RSAKeyPair{
	public:
		RSAKeyPair();
		RSAKeyPair(CryptoPP::RSA::PrivateKey privateKey, CryptoPP::RSA::PublicKey publicKey);
		void generateKeys(int size);
		CryptoPP::RSA::PrivateKey getPrivateKey();
		CryptoPP::RSA::PublicKey getPublicKey();

	private:
		CryptoPP::RSA::PrivateKey privateKey;
		CryptoPP::RSA::PublicKey publicKey;
	};
}

#endif // !DENARIUS_RSA_KEY_PAIR