#include "block.h"

using namespace denarius;

block::block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index){
	this->index = index;
	this->transactions = transactions;
	this->time = time;
	this->prev = "";
	this->nonce = 0;
	this->gym = calculateGym();
	this->hash = this->calculateHash();
}

std::string block::calculateGym() {
	return "24 hr";
}

std::string block::calculateHash() {
	std::string hashTransactions = "";

	for (auto i : this->transactions) {
		hashTransactions.append(i.hash);
	}

	std::string hashString = this->time + hashTransactions + this->gym + this->prev + std::to_string(this->nonce);
	std::string re = dCrypto::SHA3512HashString(hashString);
	std::cout << re << std::endl;

	return re;
}

bool block::mineBlock(int difficulty) {
	std::string hashPuzzle = "";
	for (int i = 0; i < difficulty; i++) {
		hashPuzzle.append(std::to_string(i));
	}
	while (this->hash.rfind(hashPuzzle, 0) != 0) {
		this->nonce++;
		this->hash = this->calculateHash();
		std::cout << "Nonce: " << std::to_string(this->nonce) << "\nHash: " << this->hash << "\n\n";
	}

	return true;
}

bool block::hasValidTransactions() {
	for (auto i : this->transactions) {
		if (!i.isValidTransaction()) {
			return false;
		}
	}
	return true;
}

nlohmann::json block::JSONEncode() {
	nlohmann::json json;
	json["index"] = this->index;
	
	std::vector<nlohmann::json> transactionsJSON;
	for (auto i : this->transactions) {
		transactionsJSON.push_back(i.JSONEncode());
	}
	json["transactions"] = transactionsJSON;
	json["time"] = this->time;
	json["previous block"] = this->prev;
	json["nonce"] = this->nonce;
	json["gym"] = this->gym;
	json["hash"] = this->hash;
	return json;
}

bool denarius::block::setPrev(std::string hash)
{
	if (this->prev.empty()) {
		this->prev = hash;
		return true;
	}
	else {
		return false;
	}
}
