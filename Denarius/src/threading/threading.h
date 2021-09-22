#ifndef DENARIUS_THREADING
#define DENARIUS_THREADING

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/strand.hpp>

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <iostream>


#include "../blockchain/blockchain"

namespace denarius {
    namespace threading {
    }

    namespace networking {

        namespace beast = boost::beast;         // from <boost/beast.hpp>
        namespace http = beast::http;           // from <boost/beast/http.hpp>
        namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
        namespace net = boost::asio;            // from <boost/asio.hpp>
        using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



        void fail(beast::error_code ec, char const* what)
        {
            std::cerr << (std::string(what) + ": " + ec.message() + "\n");
        }

        // Adjust settings on the stream
        template<class NextLayer>
        void setup_stream(websocket::stream<NextLayer>& ws)
        {
            // These values are tuned for Autobahn|Testsuite, and
            // should also be generally helpful for increased performance.

            websocket::permessage_deflate pmd;
            pmd.client_enable = true;
            pmd.server_enable = true;
            pmd.compLevel = 3;
            ws.set_option(pmd);

            ws.auto_fragment(false);

            // Autobahn|Testsuite needs this
            ws.read_message_max(64 * 1024 * 1024);
        }



        class async_session : public std::enable_shared_from_this<async_session>
        {
            websocket::stream<beast::tcp_stream> ws_;
            beast::flat_buffer buffer_;
            blockchain& blockchain_;

        public:
            // Take ownership of the socket
            explicit
                async_session(tcp::socket&& socket, blockchain& blockchain)
                : ws_(std::move(socket)), blockchain_(blockchain)
            {
                setup_stream(ws_);
            }

            // Start the asynchronous operation
            void
                run()
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

            void
                on_accept(beast::error_code ec)
            {
                if (ec)
                    return fail(ec, "accept");

                // Read a message
                do_read();
            }

            void
                do_read()
            {
                // Read a message into our buffer
                ws_.async_read(
                    buffer_,
                    beast::bind_front_handler(
                        &async_session::on_read,
                        shared_from_this()));
            }

            void on_read(beast::error_code ec, std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);

                // This indicates that the async_session was closed
                if (ec == websocket::error::closed)
                    return;

                if (ec)
                    fail(ec, "read");

                // Echo the message
                ws_.text(ws_.got_text());
                if (beast::buffers_to_string(buffer_.data()) == "Awaiting Blockchain") {

                    beast::flat_buffer b;
                    beast::ostream(b) << blockchain_.chainJSONEncode().dump(4);

                    ws_.async_write(
                        b.data(),
                        beast::bind_front_handler(
                            &async_session::on_write,
                            shared_from_this()));
                }
                else if (beast::buffers_to_string(buffer_.data()).rfind("Mine pending ", 0) == 0) {
                    bool result = blockchain_.minePendingTransactions(beast::buffers_to_string(buffer_.data()).substr(13, beast::buffers_to_string(buffer_.data()).size() -1 ));
                    beast::flat_buffer b;
                    if (result) {
                        beast::ostream(b) << "Mining complete.";
                    }
                    else {
                        beast::ostream(b) << "Could not mine new blocks.";
                    }
                    ws_.async_write(
                        b.data(),
                        beast::bind_front_handler(
                            &async_session::on_write,
                            shared_from_this()));

                }
                else if (beast::buffers_to_string(buffer_.data()).rfind("Transaction ", 0) == 0) {
                    std::vector<std::string> elems;
                    std::string Transaction = beast::buffers_to_string(buffer_.data()).substr(12, beast::buffers_to_string(buffer_.data()).size() - 1);
                    std::istringstream iss(Transaction);
                    std::string item;
                    auto params = std::back_inserter(elems);

                    beast::flat_buffer b;
                    while (std::getline(iss, item, ' ')) {
                        *params++ = item;
                    }
                    try {
                        blockchain_.pendingTransactions.push_back(denarius::transaction(elems[0], elems[1], stoi(elems[2])));
                        beast::ostream(b) << "Transaction added:\n\tSender: " << elems[0] << "\n\tReceiver: "<<elems[1] << "\n\tAmount: " << elems[2];
                    }
                    catch (std::exception& e) {
                        beast::ostream(b) << "Could not mine new blocks.";
                    }
                    ws_.async_write(
                        b.data(),
                        beast::bind_front_handler(
                            &async_session::on_write,
                            shared_from_this()));
                }
                else {
                    ws_.async_write(
                        buffer_.data(),
                        beast::bind_front_handler(
                            &async_session::on_write,
                            shared_from_this()));
                }
            }

            void on_write(beast::error_code ec, std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);

                if (ec)
                    return fail(ec, "write");

                // Clear the buffer
                buffer_.consume(buffer_.size());

                // Do another read
                do_read();
            }
        };



        class async_listener : public std::enable_shared_from_this<async_listener>
        {
            net::io_context& ioc_;
            tcp::acceptor acceptor_;
            blockchain& blockchain_;

        public:
            async_listener(net::io_context& ioc, tcp::endpoint endpoint, blockchain& blockchain)
                : ioc_(ioc), acceptor_(net::make_strand(ioc)), blockchain_(blockchain)
            {
                beast::error_code ec;

                // Open the acceptor
                acceptor_.open(endpoint.protocol(), ec);
                if (ec)
                {
                    fail(ec, "open");
                    return;
                }

                // Allow address reuse
                acceptor_.set_option(net::socket_base::reuse_address(true), ec);
                if (ec)
                {
                    fail(ec, "set_option");
                    return;
                }

                // Bind to the server address
                acceptor_.bind(endpoint, ec);
                if (ec)
                {
                    fail(ec, "bind");
                    return;
                }

                // Start listening for connections
                acceptor_.listen(net::socket_base::max_listen_connections, ec);
                if (ec)
                {
                    fail(ec, "listen");
                    return;
                }
            }

            // Start accepting incoming connections
            void run()
            {
                do_accept();
            }

        private:
            void do_accept()
            {
                // The new connection gets its own strand
                acceptor_.async_accept(
                    net::make_strand(ioc_),
                    beast::bind_front_handler(
                        &async_listener::on_accept,
                        shared_from_this()));
            }

            void on_accept(beast::error_code ec, tcp::socket socket)
            {
                if (ec)
                {
                    fail(ec, "accept");
                }
                else
                {
                    // Create the async_session and run it
                    std::make_shared<async_session>(std::move(socket), blockchain_)->run();
                }

                // Accept another connection
                do_accept();
            }
        };
    }
}

#endif // !DENARIUS_THREADING
