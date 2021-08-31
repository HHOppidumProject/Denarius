#ifndef DENARIUS_BLOCKCHAIN
#define DENARIUS_BLOCKCHAIN

#include <string>
#include <nlohmann/json.hpp>

#include "block.h"

namespace denarius {
	class blockchain {
	public:
		blockchain();
		void registerNode(std::string address);
		void resolveConflicts();
		void minePendingTransactions(std::string miner);
		void addTransaction(std::string sender, std::string receiver, std::string keyString, std::string senderKey);
		denarius::block getLastBlock();
		denarius::block addGenesisBlock();
		bool isValidChain();
		std::string generateKeys();
		nlohmann::json chainJSONEncode();
		blockchain chainJSONDencode(nlohmann::json chainJSON);
		float getBalance(std::string person);
	};
}

#endif // !DENARIUS_BLOCKCHAIN

