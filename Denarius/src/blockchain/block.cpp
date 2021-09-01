#include "block.h"

using namespace denarius;

block::block(std::vector<denarius::transaction> transactions, time_t time, unsigned long index){
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

	return "";
}