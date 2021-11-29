#include "blockchain.h"

denarius::blockchain::blockchain()
{
	this->chain = {this->addGenesisBlock()};
	this->pendingTransactions = {};
	this->difficulty = 3;
	this->minerRewards = 50;
	this->blockSize = 10;
	this->nodes = {};
}

dCrypto::RSAKeyPair denarius::blockchain::generateKeys()
{
	if (std::filesystem::exists(std::filesystem::path(dCrypto::RSAKeyPair::pubKeyPath)) && 
		std::filesystem::exists(std::filesystem::path(dCrypto::RSAKeyPair::privKeyPath))) {
		dCrypto::RSAKeyPair key(true);
		return key;
	}
	else {
		dCrypto::RSAKeyPair key;
		key.saveKeys();
		return key;
	}
}

void denarius::blockchain::registerNode(std::string address) {
	denarius::node node_(denarius::network::parse::urlparse(address)["netloc"], denarius::network::parse::urlparse(address)["port"]);
	if (!nodes.empty()) {
		bool isAlreadyRegistered = false;
		size_t i = 0;
		while (!isAlreadyRegistered && i < nodes.size()) {
			if (this->nodes[i] == node_) {
				isAlreadyRegistered = 1;
			}
			else {
				++i;
			}
		}
		if (isAlreadyRegistered) return;
	}

	this->nodes.push_back(node_);
	this->sendToNode(node_, std::string("Update blockchain ").append(this->chainJSONEncode().dump(0)).c_str());

	auto nodesJSON = this->nodeJSONEncode();
	for (size_t i = 0; i < nodesJSON.size(); i++) {
		if (nodesJSON[i] == node_.JSONEncode()) {
			nodesJSON.erase(i);
			break;
		}
	}
	this->sendToNode(node_, std::string("Update nodes ").append(nodesJSON.dump(0)).c_str());
}

void denarius::blockchain::registerNode(denarius::node node_)
{
	bool isAlreadyRegistered = false;
	size_t i = 0;
	while (!isAlreadyRegistered) {
		if (this->nodes[i] == node_) {
			isAlreadyRegistered = 1;
		}
		else {
			++i;
		}
	}
	if (isAlreadyRegistered) return;


	this->nodes.push_back(node_);
	this->sendToNode(node_, std::string("Update blockchain ").append(this->chainJSONEncode().dump(0)).c_str());

	auto nodesJSON = this->nodeJSONEncode();
	for (size_t i = 0; i < nodesJSON.size(); i++) {
		if (nodesJSON[i] == node_.JSONEncode()) {
			nodesJSON.erase(i);
			break;
		}
	}
	this->sendToNode(node_, std::string("Update nodes ").append(nodesJSON.dump(0)).c_str());
}

std::string denarius::blockchain::sendToNode(denarius::node node, std::string message) {
	boost::asio::io_context ioc;
	auto session = std::make_shared<denarius::network::client::session>(ioc);
	session->run(node["netloc"].c_str(), node["port"].c_str(), message.c_str());
	ioc.run();
	return boost::beast::buffers_to_string(session->buffer_.data());
}

void denarius::blockchain::resolveConflicts()
{
	auto neighbours = this->nodes;
	nlohmann::json newChain = {};

	size_t maxLen = this->chain.size();

	for (denarius::node node : neighbours) {
		
		auto res = this->compareBlockchain(this->sendToNode(node, "Awaiting Blockchain"), maxLen);
		
		maxLen = res.first;
		if (!res.second.empty()) newChain = res.second;
	}

	if (!newChain.empty()) {
		this->chain = this->chainJSONDecode(newChain);
		std::cout << this->chainJSONEncode().dump(4) << std::endl;
	}
	else {
		for (denarius::node node : neighbours) {
			this->sendToNode(node, std::string("Update blockchain ").append(this->chainJSONEncode().dump(0)).c_str());
		}
	}
}

std::pair<size_t, nlohmann::json> denarius::blockchain::compareBlockchain(std::string newBlockchain, size_t maxLen) {
	nlohmann::json response = nlohmann::json::parse(newBlockchain);
	nlohmann::json newChain = {};
	size_t length = response.size();
	

	if ((length > maxLen) && this->isValidChain()) {
		maxLen = length;
		newChain = response;
	}
	else {
		std::cout << "Blockchain shorter or ours is invalid:\n\tValidity: " << std::boolalpha << this->isValidChain() << "\n\tLength: " << length << std::endl;
	}

	return std::pair<size_t, nlohmann::json> {maxLen, newChain};
}

bool denarius::blockchain::updateBlockchain(std::string newBlockchain) {
	std::cout << "Received new blockchain: " << newBlockchain << std::endl;
	auto res = this->compareBlockchain(newBlockchain, this->chain.size());

	if (!res.second.empty()) {
		this->chain = this->chainJSONDecode(res.second);
		std::cout << this->chainJSONEncode().dump(4) << std::endl;
		return true;
	}
	return false;
}

bool denarius::blockchain::updateNodes(std::string nodesJSON) {
	for (auto i : nlohmann::json::parse(nodesJSON)) {
		this->registerNode(denarius::node(i["netloc"], i["port"]));
	}
	return true;
}

bool denarius::blockchain::minePendingTransactions(std::string miner) {
	size_t lenPT = this->pendingTransactions.size();
	if (lenPT <= 1) {
		return false;
	}
	else {
		for (size_t i = 0; i <= lenPT; i += 10) {
			size_t end = i + 10;
			if (end >= lenPT){
				end = lenPT;
			}
			std::vector<denarius::transaction> TransactionSlice(this->pendingTransactions.begin()+i, this->pendingTransactions.begin()+end);
			denarius::block newBlock = denarius::block(TransactionSlice, dtime::currentTime(), this->chain.size(), this->getLastBlock().gethash());
			newBlock.mineBlock(this->difficulty);
			this->chain.push_back(newBlock);
		}
		this->pendingTransactions.clear();
		this->pendingTransactions.push_back(denarius::transaction("Miner Rewards", miner, this->minerRewards));
		this->resolveConflicts();
	}
	return true;
}

denarius::block denarius::blockchain::getLastBlock()
{
	if (this->chain.empty()) {
		this->chain.push_back(this->addGenesisBlock());
	}

	return this->chain.back();
}

denarius::block denarius::blockchain::addGenesisBlock() {
	std::vector<denarius::transaction> t = { 
		denarius::transaction("The Treasury of the Imperium Romanum", "The Treasury of the Imperium Romanum", 1000000) 
	};
	denarius::block genesis(t, dtime::currentTime(), 0, "N/A");
	genesis.mineBlock(this->difficulty);
	return genesis;
}

bool denarius::blockchain::isValidChain()
{
	for (size_t i = 1; i < this->chain.size(); ++i) {
		auto block1 = this->chain[i - 1];
		std::cout << "block1 : " << block1.getPrev() << std::endl;
		auto block2 = this->chain[i];
		
		if (!block2.hasValidTransactions()) {
			std::cout << "Block2 has not got valid transactions" << std::endl;
			return false;
		}

		if (block2.gethash() != block2.calculateHash()) {
			std::cout << "Block2 has not got a valid hash" << std::endl;
			return false;
		}

		if (block2.getPrev() != block1.gethash()) {
			std::cout << "Block2 has not got valid previous" << std::endl;
			return false;
		}
	}
	return true;
}

nlohmann::json denarius::blockchain::chainJSONEncode() {
	nlohmann::json json;
	for (auto i : this->chain) {
		json.push_back(i.JSONEncode());
	}
	return json;
}

std::vector<denarius::block> denarius::blockchain::chainJSONDecode(nlohmann::json chainJSON)
{
	std::vector<denarius::block> chain_ = {};

	for (nlohmann::json blockJSON : chainJSON) {

		std::vector<denarius::transaction> tArr = {};
		for (nlohmann::json transactionJSON : blockJSON["transactions"]) {
			tArr.push_back(
				denarius::transaction(
					transactionJSON["sender"],
					transactionJSON["receiver"],
					transactionJSON["amount"],
					transactionJSON["time"],
					transactionJSON["hash"],
					transactionJSON["signature"]
				)
			);
		}
		try {
			chain_.push_back(denarius::block(tArr, blockJSON["time"], blockJSON["index"], blockJSON["previous block"], blockJSON["hash"], blockJSON["nonce"], blockJSON["exchange"]));
		}
		catch (nlohmann::detail::type_error& e) {
			std::cerr << "Unable to update chain :";
			std::cerr << e.what() << std::endl;
			return this->chain;
		}

	}

	return chain_;
}

nlohmann::json denarius::blockchain::nodeJSONEncode() {
	nlohmann::json json;
	for (auto i : this->nodes) {
		json.push_back(i.JSONEncode());
	}
	return json;
}

std::vector<std::string> denarius::blockchain::getNodes()
{
	std::vector<std::string> nodes_;
	for (auto& node : this->nodes) {
		nodes_.push_back(node["netloc"]);
	}
	return nodes_;
}
