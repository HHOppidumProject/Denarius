#ifndef DENARIUS_CLIENT_ASYNC_SESSION
#define DENARIUS_CLIENT_ASYNC_SESSION

#include "network.hpp"

namespace denarius {
	namespace network {
		namespace client {
			class session : public std::enable_shared_from_this<session>
			{
				tcp::resolver resolver_;
				websocket::stream<beast::tcp_stream> ws_;
				std::string host_;
				std::string text_;

			public:
				beast::flat_buffer buffer_;
				// Resolver and socket require an io_context
				explicit session(net::io_context& ioc)
					: resolver_(net::make_strand(ioc))
					, ws_(net::make_strand(ioc))
				{
				}

				// Start the asynchronous operation
				void run(char const* host, char const* port, char const* text);

				void on_resolve(beast::error_code ec, tcp::resolver::results_type results);

				void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);

				void on_handshake(beast::error_code ec);

				void on_write(beast::error_code ec, std::size_t bytes_transferred);

				void on_read(beast::error_code ec, std::size_t bytes_transferred);

				void on_close(beast::error_code ec);
			};
		}
	}
}

#endif // !DENARIUS_CLIENT_ASYNC_SESSION
