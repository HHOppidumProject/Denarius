#include "crypto.h"

std::string dCrypto::SHA3512HashString(std::string str) {
	CryptoPP::SHA3_512 sha3;
	std::string result, digest;
	CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));
	sha3.Update((const CryptoPP::byte*)str.data(), str.size());
	digest.resize(sha3.DigestSize());
	sha3.Final((CryptoPP::byte*)&digest[0]);
	CryptoPP::StringSource(digest, true, new CryptoPP::Redirector(encoder));
	return result;

}