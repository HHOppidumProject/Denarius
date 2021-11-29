#ifndef DENARIUS_USER
#define DENARIUS_USER

#include <string>
#include <regex>
#include <iostream>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>

#include "../crypto/crypto"

namespace denarius {
	class user {
	public:
		user(std::string username, std::string password, std::string email, std::string name, dCrypto::RSAKeyPair key);
		bool verifyEmail(std::string email);

	private:
		std::string hashPassword(std::string password);
		std::string username, password, email, name;
		dCrypto::RSAKeyPair key;
	};
}

#endif // !DENARIUS_USER