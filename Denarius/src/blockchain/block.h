#ifndef DENARIUS_BLOCK
#define DENARIUS_BLOCK

#include <string>
#include <ctime>
#include <vector>

#include <nlohmann/json.hpp>

#include "transaction.h"

#include "../crypto/crypto"

namespace denarius {
	class block {
	public:
		block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index);
		block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index, std::string prev);
		block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index, std::string prev, std::string hash, int nonce, std::string exchange);
		std::string calculateExchange();
		std::string calculateHash();
		bool mineBlock(int difficulty);
		bool hasValidTransactions();
		nlohmann::json JSONEncode();

		std::string gethash() { return this->hash; }
		std::string getPrev();
	private:
		unsigned long index;
		std::vector<denarius::transaction> transactions;
		std::string time;
		std::string prev;
		int nonce;
		std::string exchange;
		std::string hash;
	};
}

#endif // !DENARUUS_BLOCK
