#include "rsaKeyPair.h"

dCrypto::RSAKeyPair::RSAKeyPair() {
	this->generateKeys(3072);
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

CryptoPP::RSA::PrivateKey dCrypto::RSAKeyPair::getPrivateKey() {
	return this->privateKey;
}

CryptoPP::RSA::PublicKey dCrypto::RSAKeyPair::getPublicKey() {
	return this->publicKey;
}