#include "user.h"

denarius::user::user(std::string username, std::string password, std::string email, std::string name, dCrypto::RSAKeyPair key)
{
	if (this->verifyEmail(email)) {
		this->username = username;
		this->email = email;
		this->name = name;
		this->key = key;
		this->password = this->hashPassword(password);
	}
	else {
		std::cout << "Failed to create user, email is invalid." << std::endl;
	}
}

bool denarius::user::verifyEmail(std::string email)
{
	try {
		std::regex regex("[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,64}");
		return std::regex_match(email, regex);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return false;
}

std::string denarius::user::hashPassword(std::string password) {
	CryptoPP::SHA3_512 sha3;
	CryptoPP::SHA256 sha2;
	std::string result, cipher, digest, intermittent;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(intermittent));
	
	//Get public key cipher
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(this->key.getPublicKey());
	CryptoPP::StringSource ss(this->username + this->password, true, new CryptoPP::PK_EncryptorFilter(rng, e, new CryptoPP::StringSink(cipher)));

	//SHA2-256
	password = password + cipher;
	sha2.Update((const CryptoPP::byte*)password.data(), password.size());
	digest.resize(sha2.DigestSize());
	sha2.Final((CryptoPP::byte*)&digest[0]);
	CryptoPP::StringSource(digest, true, new CryptoPP::Redirector(encoder));
	
	//SHA3-512
	encoder.Detach(new CryptoPP::StringSink(result));
	intermittent = result + cipher;
	for (int i = 0; i < 25; i++) {
		result.clear();
		digest.clear();
		sha3.Update((const CryptoPP::byte*)intermittent.data(), intermittent.size());
		digest.resize(sha3.DigestSize());
		sha3.Final((CryptoPP::byte*)&digest[0]);
		CryptoPP::StringSource(digest, true, new CryptoPP::Redirector(encoder));
		intermittent = result;
	}
	return result;
}