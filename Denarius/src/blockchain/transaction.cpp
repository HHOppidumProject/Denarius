#include "transaction.h"

using namespace denarius;

transaction::transaction(std::string sender, std::string receiver, long amt) {
	this->sender = sender;
	this->receiver = receiver;
	this->amt = amt;
	this->currentTime = dtime::currentTime();
	this->hash = this->calculateHash();
}

std::string transaction::calculateHash() {
	CryptoPP::SHA3_512 hash;
	std::string hashstring;
	std::string digest;
	std::string result;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));

	hashstring = this->sender + this->receiver + std::to_string(this->amt) + this->currentTime;
	hash.Update((const CryptoPP::byte*)hashstring.data(), hashstring.size());
	digest.resize(hash.DigestSize());
	hash.Final((CryptoPP::byte*)&digest[0]);
	CryptoPP::StringSource(digest, true, new CryptoPP::Redirector(encoder));
	return result;
}

bool transaction::isValidTransaction() {
	if (this->hash != this->calculateHash()) {
		return false;
	}
	if (this->sender == this->receiver) {
		return false;
	}
	if (this->signature == "" or this->signature.length() == 0) {
		return false;
	}
	return true;
}

bool transaction::signTransaction(dCrypto::RSAKeyPair key, dCrypto::RSAKeyPair senderKey) {
	if (this->hash != this->calculateHash()) {
		return false;
	}

	//Sign Transaction
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(senderKey.getPrivateKey());
	CryptoPP::StringSource ss1(this->hash, true, new CryptoPP::SignerFilter(rng, signer, new CryptoPP::StringSink(this->signature)));

	//Verify Transaction
	try {
		CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(key.getPublicKey());
		CryptoPP::StringSource ss2(this->hash + this->signature, true, new CryptoPP::SignatureVerificationFilter(verifier, NULL, CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION));
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		this->signature = "";
		return false;
	}
	return true;
}

nlohmann::json transaction::JSONEncode() {
	nlohmann::json json;
	json["sender"] = this->sender;
	json["receiver"] = this->receiver;
	json["amount"] = this->amt;
	json["time"] = this->currentTime;
	json["hash"] = this->hash;
	return json;
}