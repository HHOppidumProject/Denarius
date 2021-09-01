#include "blockchain.h"

denarius::blockchain::blockchain()
{
	this->chain = {};
	this->pendingTransactions = {};
	this->difficulty = 2;
	this->minerRewards = 50;
	this->blockSize = 10;
}

std::string denarius::blockchain::generateKeys()
{
	// Add key generation and saving
	//CryptoPP::AutoSeededRandomPool rnd;
	//CryptoPP::RSA::PrivateKey rsaPriv;
	//rsaPriv.GenerateRandomWithKeySize(rnd, 3072);
	//CryptoPP::RSA::PublicKey rsaPub(rsaPriv);

	////Save keys
	//CryptoPP::FileSink privKey("Key");
	//CryptoPP::FileSink pubKey("Key.pub");
	//std::string result;
	//CryptoPP::StringSink ss(result);
	//CryptoPP::ByteQueue queue;
	//
	////Private key save
	//rsaPriv.DEREncodePrivateKey(queue);
	//queue.CopyTo(privKey);
	//privKey.MessageEnd();
	//queue.Clear();

	////Pub key save
	//rsaPub.DEREncodePublicKey(queue);
	//queue.CopyTo(pubKey);
	//pubKey.MessageEnd();

	//
	//rsaPub.BERDecodePublicKey(queue, false, queue.MaxRetrievable());
	//queue.CopyTo(ss);
	//ss.MessageEnd();
	//queue.Clear();
	//return result;
	return "Placeholder";
}
