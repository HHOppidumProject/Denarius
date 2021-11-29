#include "block.h"

using namespace denarius;

block::block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index){
	this->index = index;
	this->transactions = transactions;
	this->time = time;
	this->prev = "";
	this->nonce = 0;
	this->exchange = calculateExchange();
	this->hash = this->calculateHash();
}

denarius::block::block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index, std::string prev)
{
	this->index = index;
	this->transactions = transactions;
	this->time = time;
	this->prev = prev;
	this->nonce = 0;
	this->exchange = calculateExchange();
	this->hash = this->calculateHash();
}

denarius::block::block(std::vector<denarius::transaction> transactions, std::string time, unsigned long index, std::string prev, std::string hash, int nonce, std::string exchange)
{
	this->index = index;
	this->transactions = transactions;
	this->time = time;
	this->prev = prev;
	this->nonce = nonce;
	this->exchange = exchange;
	this->hash = hash;
}

std::string block::calculateExchange() {
	return "Imperium Romanum Solidus Node and Exchange";
}

std::string block::calculateHash() {
	std::string hashTransactions = "";

	for (auto i : this->transactions) {
		hashTransactions.append(i.hash);
	}

	std::string hashString = this->time + hashTransactions + this->exchange + this->prev + std::to_string(this->nonce);
	for (int i = 0; i < 15; i++) {
		std::string re = dCrypto::SHA3512HashString(hashString);
		hashString = re;
	}
	return hashString;
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
	json["exchange"] = this->exchange;
	json["hash"] = this->hash;
	return json;
}

std::string denarius::block::getPrev()
{
	return this->prev;
}
