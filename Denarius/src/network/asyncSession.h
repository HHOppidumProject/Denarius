#ifndef DENARIUS_ASYNC_SESSION
#define DENARIUS_ASYNC_SESSION

#include "network.hpp"
#include "../blockchain/blockchain"

namespace denarius {
	namespace network {

		namespace beast = boost::beast;         // from <boost/beast.hpp>
		namespace http = beast::http;           // from <boost/beast/http.hpp>
		namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
		namespace net = boost::asio;            // from <boost/asio.hpp>
		using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

		class async_session : public std::enable_shared_from_this<async_session>
		{
			websocket::stream<beast::tcp_stream> ws_;
			beast::flat_buffer buffer_;
			denarius::blockchain& blockchain_;

		public:
			// Take ownership of the socket
			explicit async_session(tcp::socket&& socket, denarius::blockchain& blockchain) : ws_(std::move(socket)), blockchain_(blockchain)
			{
				setup_stream(ws_);
			}

			// Start the asynchronous operation
			void run();

			void on_accept(beast::error_code ec);

			void do_read();

			void on_read(beast::error_code ec, std::size_t bytes_transferred);

			void on_write(beast::error_code ec, std::size_t bytes_transferred);
		};


	}
}

#endif // !DENARIUS_ASYNC_SESSION
