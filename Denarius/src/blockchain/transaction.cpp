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

bool transaction::signTransaction(std::string key, std::string senderKey) {
	if (this->hash != this->calculateHash()) {
		return false;
	}
	if (false) {

	}
}