#include "asyncSession.h"

void denarius::network::async_session::run()
{
    // Set suggested timeout settings for the websocket
    ws_.set_option(
        websocket::stream_base::timeout::suggested(
            beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res)
        {
            res.set(http::field::server, std::string(
                BOOST_BEAST_VERSION_STRING) + "-Async");
        }));

    // Accept the websocket handshake
    ws_.async_accept(
        beast::bind_front_handler(
            &async_session::on_accept,
            shared_from_this()));
}

void denarius::network::async_session::on_accept(beast::error_code ec)
{
    if (ec)
        return denarius::network::fail(ec, "accept");

    // Read a message
    do_read();
}

void denarius::network::async_session::do_read()
{
    // Read a message into our buffer
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(
            &async_session::on_read,
            shared_from_this()));
}

void denarius::network::async_session::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the async_session was closed
    if (ec == websocket::error::closed)
        return;

    if (ec)
        denarius::network::fail(ec, "read");

    // Echo the message
    ws_.text(ws_.got_text());
    beast::flat_buffer b;
    std::string incomingMessage = beast::buffers_to_string(buffer_.data());
    if (incomingMessage == "Awaiting Blockchain") {
        beast::ostream(b) << blockchain_.chainJSONEncode().dump(4);

    }
    else if (incomingMessage.rfind("Get nodes", 0) == 0) {
        auto nodes = this->blockchain_.getNodes();
        beast::ostream(b) << "Current Nodes:\n\t" << std::accumulate(nodes.begin(), nodes.end(), std::string{},
            [](std::string prevRes, std::string toAddToString) {
                return prevRes.append("\n\t").append(toAddToString);
            });;

    }
    else if (incomingMessage.rfind("Update nodes ", 0) == 0) {
        std::string nodesList = incomingMessage.substr(13, incomingMessage.size() - 1);
        this->blockchain_.updateNodes(nodesList);
        beast::ostream(b) << "Updated Nodes";
    }
    else if (incomingMessage.rfind("Update blockchain ", 0) == 0) {
        std::string newBlockchain = incomingMessage.substr(18, incomingMessage.size() - 1);
        if (blockchain_.updateBlockchain(newBlockchain)) {
            beast::ostream(b) << "Updated Blockchain";
        }
        else {
            beast::ostream(b) << "did not update Blockchain";
        }
    }
    else if (incomingMessage.rfind("Register node ", 0) == 0) {
        try {
            std::string nodeToRegister = incomingMessage.substr(14, incomingMessage.size() - 1);
            blockchain_.registerNode(nodeToRegister);
            auto nodes = blockchain_.getNodes();
            beast::ostream(b) << "Node added:\n\t" << nodeToRegister <<
                "\nCurrent Nodes:\n\t" << std::accumulate(nodes.begin(), nodes.end(), std::string{}, 
                    [](std::string prevRes, std::string toAddToString) {
                        return prevRes.append("\n\t").append(toAddToString);
                    });
        }
        catch (std::exception& e) {
            beast::ostream(b) << "Unable to register node:\n\tError: " << e.what();
        }
        
    }
    else if (incomingMessage.rfind("Mine pending ", 0) == 0) {
        bool result = blockchain_.minePendingTransactions(incomingMessage.substr(13, incomingMessage.size() - 1));
        if (result) {
            beast::ostream(b) << "Mining complete.";
        }
        else {
            beast::ostream(b) << "Could not mine new blocks.";
        }
    }
    else if (incomingMessage.rfind("Transaction ", 0) == 0) {
        std::vector<std::string> elems;
        std::string Transaction = incomingMessage.substr(12, incomingMessage.size() - 1);
        std::istringstream iss(Transaction);
        std::string item;

        if ((Transaction.find("-s", 0) != std::string::npos) ||
            (Transaction.find("-r", 0) != std::string::npos) ||
            (Transaction.find("-a", 0) != std::string::npos)) {
            auto locOfSender = Transaction.find("-s", 0);
            auto locOfReceiver = Transaction.find("-r", 0);
            auto locOfAmount = Transaction.find("-a", 0);

            if (locOfReceiver > locOfSender) {
                if (locOfReceiver > locOfAmount) {
                    elems.push_back(Transaction.substr(locOfSender + 3, locOfAmount - 4));
                    elems.push_back(Transaction.substr(locOfReceiver + 3, Transaction.length() - 1));
                    elems.push_back(Transaction.substr(locOfAmount + 3, locOfReceiver - (locOfAmount + 4)));
                }
                else {
                    elems.push_back(Transaction.substr(locOfSender + 3, locOfReceiver - 4));
                    elems.push_back(Transaction.substr(locOfReceiver + 3, locOfAmount - (locOfReceiver + 4)));
                    elems.push_back(Transaction.substr(locOfAmount + 3, Transaction.length() - 1));
                }
            }

            try {
                blockchain_.pendingTransactions.push_back(denarius::transaction(elems[0], elems[1], stoi(elems[2])));
                beast::ostream(b) << "Transaction added:\n\tSender: " << elems[0] << "\n\tReceiver: " << elems[1] << "\n\tAmount: " << elems[2];
            }
            catch (std::exception) {
                beast::ostream(b) << "Could not add transaction.\n\tFormat: Transaction -s <sender> -r <receiver> -a <amount>\n";
            }
        }
        else {
            try {
                beast::ostream(b) << "Could not add transaction.\n\tFormat: Transaction -s <sender> -r <receiver> -a <amount>\n";
            }
            catch (std::exception) {
                beast::ostream(b) << "Could not add transaction.\n\tFormat: Transaction -s <sender> -r <receiver> -a <amount>\n";
            }
        }
    }
    else {
        beast::ostream(b) << incomingMessage;
    }
    ws_.async_write(
        b.data(),
        beast::bind_front_handler(
            &async_session::on_write,
            shared_from_this()));
}

void denarius::network::async_session::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return denarius::network::fail(ec, "write");

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    do_read();
}
