#ifndef DENARIUS_RSA_KEY_PAIR
#define DENARIUS_RSA_KEY_PAIR

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/pem.h>


namespace dCrypto {
	class RSAKeyPair{
	public:
		RSAKeyPair(bool loadKey = false);
		RSAKeyPair(CryptoPP::RSA::PrivateKey privateKey, CryptoPP::RSA::PublicKey publicKey);
		void generateKeys(int size);
		void saveKeys();
		CryptoPP::RSA::PrivateKey getPrivateKey();
		CryptoPP::RSA::PublicKey getPublicKey();

		void loadKeys();
		void loadKey(std::string keyDest, bool isPrivKey = false);

		static const std::string pubKeyPath;
		static const std::string privKeyPath;

	private:
		CryptoPP::RSA::PrivateKey privateKey;
		CryptoPP::RSA::PublicKey publicKey;
	};
}

#endif // !DENARIUS_RSA_KEY_PAIR