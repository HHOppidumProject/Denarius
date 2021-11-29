#ifndef DENARIUS_BLOCKCHAIN
#define DENARIUS_BLOCKCHAIN

#include <string>
#include <filesystem>

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <nlohmann/json.hpp>

#include "block.h"
#include "node.h"

#include "../crypto/crypto"
#include "../network/parse.h"
#include "../network/clientAsyncSession.h"

namespace denarius {
	class blockchain {
	public:
		blockchain();
		bool isValidChain();
		dCrypto::RSAKeyPair generateKeys();
		nlohmann::json chainJSONEncode();
		std::vector<denarius::block> chainJSONDecode(nlohmann::json chainJSON);

		/* ------------------------------------------------ NODE FUNCTIONS ------------------------------------------------
		* Functions to handle nodes
		*/
		void registerNode(std::string address);
		void registerNode(denarius::node node_);
		std::string sendToNode(denarius::node node, std::string message);
		bool updateNodes(std::string nodesJSON);
		nlohmann::json nodeJSONEncode();
		void resolveConflicts();
		std::vector<std::string> getNodes();
		// ---------------------------------------------- END NODE FUNCTIONS ----------------------------------------------

		/* ------------------------------------------------ BLOCK FUNCTIONS -----------------------------------------------
		*
		*/
		std::pair<size_t, nlohmann::json> compareBlockchain(std::string newBlockchain, size_t maxLen);
		bool minePendingTransactions(std::string miner);
		denarius::block addGenesisBlock();
		denarius::block getLastBlock();
		bool updateBlockchain(std::string newBlockchain);

		// --------------------------------------------- END BLOCK FUNCTIONS ----------------------------------------------

		/* --------------------------------------------- TRANSACTION FUNCTIONS --------------------------------------------
		*
		*/
		void addTransaction(std::string sender, std::string receiver, std::string keyString, std::string senderKey);
		float getBalance(std::string person);

		// ------------------------------------------- END TRANSACTION FUNCTIONS ------------------------------------------

	private:
		std::vector<denarius::block> chain;
		std::vector<denarius::transaction> pendingTransactions;
		int difficulty;
		int minerRewards;
		int blockSize;
		std::vector<denarius::node> nodes;
	};
}

#endif // !DENARIUS_BLOCKCHAIN

