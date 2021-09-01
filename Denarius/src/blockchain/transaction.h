#ifndef DENARIUS_TRANSACATION
#define DENARIUS_TRANSACATION

#include <string>
#include <ctime>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#include "../time/time.h"

namespace denarius {
	class transaction {
	public:
		transaction(std::string sender, std::string receiver, long amt);

		std::string calculateHash();
		bool isValidTransaction();
		bool signTransaction(std::string key, std::string senderKey);

		std::string hash;
	private:
		std::string sender;
		std::string receiver;
		long amt;
		std::string currentTime;
		std::string signature;
	};
}

#endif // !DENARIUS_TRANSACTION