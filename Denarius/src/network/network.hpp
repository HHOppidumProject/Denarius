#ifndef DENARIUS_NETWORK_H
#define DENARIUS_NETWORK_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/buffers_iterator.hpp>

#include <iostream>

namespace denarius {
	namespace network {

        namespace beast = boost::beast;         // from <boost/beast.hpp>
        namespace http = beast::http;           // from <boost/beast/http.hpp>
        namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
        namespace net = boost::asio;            // from <boost/asio.hpp>
        using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

        inline void fail(beast::error_code ec, char const* what) {
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

	}
}

#endif // !DENARIUS_NETWORK_H