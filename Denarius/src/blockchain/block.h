#ifndef DENARIUS_BLOCK
#define DENARIUS_BLOCK

#include <string>
#include <ctime>
#include <vector>

#include <nlohmann/json.hpp>

#include "transaction.h"

namespace denarius {
	class block {
	public:
		block(std::vector<denarius::transaction> transactions, time_t time, unsigned long index);
		std::string calculateGym();
		std::string calculateHash();
		bool mineBlock(int difficulty);
		bool hasValidTransactions();
		nlohmann::json JSONEncode();
	private:
		unsigned long index;
		std::vector<denarius::transaction> transactions;
		time_t time;
		std::string prev;
		int nonce;
		std::string gym;
		std::string hash;
	};
}

#endif // !DENARUUS_BLOCK
