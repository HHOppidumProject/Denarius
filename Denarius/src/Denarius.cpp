// Denarius.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <thread>
#include <iostream>
#include <memory>

#include "blockchain/blockchain"
#include "network/asyncListener.h"
#include "user/user.h"

int main()
{
    denarius::blockchain Blockchain;
    dCrypto::RSAKeyPair key = Blockchain.generateKeys();
    std::cout << Blockchain.chainJSONEncode().dump(4) << std::endl;

    boost::asio::io_context ioc{ 3 };
    auto const address = boost::asio::ip::make_address("127.0.0.1");

    std::make_shared<denarius::network::async_listener>(
        ioc,
        boost::asio::ip::tcp::endpoint{
            address,
#ifdef D_PORT
            static_cast<unsigned short>(D_PORT) },
#else
            static_cast<unsigned short>(3001) },
#endif
        Blockchain)->run();
    
    std::vector<std::thread> v;
    v.reserve(3 - 1);
    
    for (auto i = 3 - 1; i > 0; --i)
        v.emplace_back(
            [&ioc]
            {
                ioc.run();
            });
    ioc.run();

}