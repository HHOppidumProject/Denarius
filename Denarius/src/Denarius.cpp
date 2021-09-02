// Denarius.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "blockchain/blockchain"
#include "user/user.h"
int main()
{
    denarius::transaction transaction("Foo", "Bar", 12);
    denarius::blockchain Blockchain;
    dCrypto::RSAKeyPair key = Blockchain.generateKeys();
    denarius::user user1("apolo49", "foo", "foo@bar.com", "Joe", key);
    //denarius::user user2("wavern64", "bar", "bar@bar.com", "Harrison", Blockchain.generateKeys());
    std::cout << "\n\n\n\n";
    Blockchain.pendingTransactions.push_back(transaction);
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.pendingTransactions.push_back(denarius::transaction("apolo49", "wavern64", 10));
    Blockchain.minePendingTransactions("apolo49");
    std::cout << Blockchain.chainJSONEncode().dump(4) << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
