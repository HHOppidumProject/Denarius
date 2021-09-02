#ifndef DENARIUS_BLOCKCHAIN
#define DENARIUS_BLOCKCHAIN

#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <nlohmann/json.hpp>
#include <python3.9/Python.h>

#include "block.h"

#include "../denCrypto/rsaKeyPair.h"

namespace denarius {
	class blockchain {
	public:
		blockchain();
		void registerNode(std::string address);
		void resolveConflicts();
		bool minePendingTransactions(std::string miner);
		void addTransaction(std::string sender, std::string receiver, std::string keyString, std::string senderKey);
		denarius::block getLastBlock();
		denarius::block addGenesisBlock();
		bool isValidChain();
		dCrypto::RSAKeyPair generateKeys();
		nlohmann::json chainJSONEncode();
		blockchain chainJSONDencode(nlohmann::json chainJSON);
		float getBalance(std::string person);
		std::vector<denarius::transaction> pendingTransactions;

	private:
		std::vector<denarius::block> chain;
		int difficulty;
		int minerRewards;
		int blockSize;
		std::vector<std::string> nodes;
	};
}

#endif // !DENARIUS_BLOCKCHAIN

