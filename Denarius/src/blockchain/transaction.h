#ifndef DENARIUS_TRANSACATION
#define DENARIUS_TRANSACATION

#include <string>
#include <ctime>

#include <nlohmann/json.hpp>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#include "../time/time"
#include "../crypto/crypto"

namespace denarius {
	class transaction {
	public:
		transaction(std::string sender, std::string receiver, long amt);
		transaction(std::string sender, std::string receiver, long amt, std::string time, std::string hash, std::string signature);

		std::string calculateHash();
		bool isValidTransaction();
		bool signTransaction(dCrypto::RSAKeyPair key, dCrypto::RSAKeyPair senderKey);
		nlohmann::json JSONEncode();
		std::string getSender() { return sender; }
		std::string getReceiver() { return receiver; }
		long getAmt() { return amt; }
		
		std::string hash;

	private:
		std::string signature;
		std::string sender;
		std::string receiver;
		long amt;
		std::string currentTime;
	};
}

#endif // !DENARIUS_TRANSACTION