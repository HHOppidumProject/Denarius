#ifndef DENARIUS_ASYNC_LISTENER
#define DENARIUS_ASYNC_LISTENER

#include "asyncSession.h"

namespace denarius {
	namespace network {


        namespace beast = boost::beast;         // from <boost/beast.hpp>
        namespace http = beast::http;           // from <boost/beast/http.hpp>
        namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
        namespace net = boost::asio;            // from <boost/asio.hpp>
        using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

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
                    denarius::network::fail(ec, "open");
                    return;
                }

                // Allow address reuse
                acceptor_.set_option(net::socket_base::reuse_address(true), ec);
                if (ec)
                {
                    denarius::network::fail(ec, "set_option");
                    return;
                }

                // Bind to the server address
                acceptor_.bind(endpoint, ec);
                if (ec)
                {
                    denarius::network::fail(ec, "bind");
                    return;
                }

                // Start listening for connections
                acceptor_.listen(net::socket_base::max_listen_connections, ec);
                if (ec)
                {
                    denarius::network::fail(ec, "listen");
                    return;
                }
            }

            // Start accepting incoming connections
            void run();

        private:
            void do_accept();

            void on_accept(beast::error_code ec, tcp::socket socket);
        };
	}
}

#endif // !DENARIUS_ASYNC_LISTENER

