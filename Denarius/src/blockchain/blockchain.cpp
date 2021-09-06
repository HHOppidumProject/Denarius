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

	dCrypto::RSAKeyPair key;
	CryptoPP::ByteQueue privQueue;
	CryptoPP::ByteQueue pubQueue;
	
	key.getPrivateKey().Save(privQueue);
	CryptoPP::FileSink file("Key");
	privQueue.CopyTo(file);
	file.MessageEnd();

	key.getPublicKey().Save(pubQueue);
	CryptoPP::FileSink file1("Key.pub");
	pubQueue.CopyTo(file1);
	file1.MessageEnd();

	return key;
}

void denarius::blockchain::registerNode(std::string address) {
	std::string path = "C:/dev/vcpkg/packages/python3_x64-windows-static/tools/python3/Lib";
	size_t size = path.size()-1;
	Py_SetPath(Py_DecodeLocale(path.c_str(), &size));
	Py_Initialize();
	PyRun_SimpleString("print('begin')");
	PyObject* myModuleString = PyUnicode_FromString("__main__");
	PyObject* myModule = PyImport_Import(myModuleString);
	PyRun_SimpleString(
		"from urllib.parse import urlparse\n"\
		"def main(address):\n"\
		"	return urlparse(address).netloc");
	PyObject* myFunction = PyObject_GetAttrString(myModule, "main");
	this->nodes.push_back(
		PyUnicode_AsUTF8(
			PyObject_CallFunction(myFunction, "s", address.c_str())
		)
	);
	Py_Finalize();
}

bool denarius::blockchain::minePendingTransactions(std::string miner) {
	size_t lenPT = this->pendingTransactions.size();
	if (lenPT <= 1) {
		return false;
	}
	else {
		for (int i = 0; i <= lenPT; i += 10) {
			int end = i + 10;
			if (end >= lenPT){
				end = lenPT;
			}
			std::vector<denarius::transaction> TransactionSlice(this->pendingTransactions.begin()+i, this->pendingTransactions.begin()+end);
			denarius::block newBlock = denarius::block(TransactionSlice, dtime::currentTime(), this->chain.size());
			std::string hashVal = this->getLastBlock().gethash();
			newBlock.setPrev(hashVal);
			newBlock.mineBlock(this->difficulty);
			this->chain.push_back(newBlock);
		}
		this->pendingTransactions.clear();
		this->pendingTransactions.push_back(denarius::transaction("Miner Rewards", miner, this->minerRewards));
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
	std::vector<denarius::transaction> t = { denarius::transaction("The Nova Roman Treasury", "The Nova Roman Treasury", 1000000) };
	denarius::block genesis(t, dtime::currentTime(), 0);
	genesis.setPrev("N/A");
	return genesis;
}

nlohmann::json denarius::blockchain::chainJSONEncode() {
	nlohmann::json json;
	for (auto i : this->chain) {
		json.push_back(i.JSONEncode());
	}
	return json;
}
