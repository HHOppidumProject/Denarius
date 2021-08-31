#ifndef DENARIUS_BLOCK
#define DENARIUS_BLOCK

#include <string>
#include <ctime>

#include <nlohmann/json.hpp>

namespace denarius {
	class block {
	public:
		block(std::string transactions, time_t time, unsigned long index);
		std::string calculateGym();
		std::string calculateHash();
		bool mineBlock(int difficulty);
		bool hasValidTransactions();
		nlohmann::json JSONEncode();
	private:
		unsigned long index;
		std::string transactions;
	};
}

#endif // !DENARUUS_BLOCK
