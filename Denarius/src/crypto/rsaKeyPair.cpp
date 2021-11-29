#include "rsaKeyPair.h"

const std::string dCrypto::RSAKeyPair::pubKeyPath = "./PermutatioClave.pub";
const std::string dCrypto::RSAKeyPair::privKeyPath = "./PermutatioClave";

dCrypto::RSAKeyPair::RSAKeyPair(bool loadKey) {
	if (loadKey) {
		this->loadKeys();
	}
	else {
		this->generateKeys(3072);
	}
}

dCrypto::RSAKeyPair::RSAKeyPair(CryptoPP::RSA::PrivateKey privateKey, CryptoPP::RSA::PublicKey publicKey) {
	this->privateKey = privateKey;
	this->publicKey = publicKey;
}

void dCrypto::RSAKeyPair::generateKeys(int size){
	CryptoPP::AutoSeededRandomPool rnd;
	this->privateKey.GenerateRandomWithKeySize(rnd, size);
	this->publicKey = CryptoPP::RSA::PublicKey(this->privateKey);
}

void dCrypto::RSAKeyPair::saveKeys()
{
	/* Crashing for unkown reason - track issue https://github.com/weidai11/cryptopp/issues/1089
	CryptoPP::FileSource fspub("./ire.pub", true);
	CryptoPP::FileSource fspriv("./ire.priv", true);
	*/
	CryptoPP::FileSink fspub(pubKeyPath.c_str(), true);
	CryptoPP::FileSink fspriv(privKeyPath.c_str(), true);
	try {
		PEM_Save(fspub, this->publicKey);
		PEM_Save(fspriv, this->privateKey);
	}
	catch (CryptoPP::FileStore::OpenErr &e) {
		std::cout << strerror(errno) << "\n";
		std::cout << e.what() << std::endl;
		exit(-1);
	}

}

void dCrypto::RSAKeyPair::loadKeys() {
	CryptoPP::FileSource fspub(pubKeyPath.c_str(), true);
	CryptoPP::FileSource fspriv(privKeyPath.c_str(), true);
	try {
		PEM_Load(fspub, this->publicKey);
		PEM_Load(fspriv, this->privateKey);
	}
	catch (CryptoPP::FileStore::OpenErr& e) {
		std::cout << strerror(errno) << "\n";
		std::cout << e.what() << std::endl;
		exit(-1);
	}
}

CryptoPP::RSA::PrivateKey dCrypto::RSAKeyPair::getPrivateKey() {
	return this->privateKey;
}

CryptoPP::RSA::PublicKey dCrypto::RSAKeyPair::getPublicKey() {
	return this->publicKey;
}