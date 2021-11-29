#include "asyncListener.h"

void denarius::network::async_listener::run()
{
    do_accept();
}

void denarius::network::async_listener::do_accept()
{
    // The new connection gets its own strand
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(
            &async_listener::on_accept,
            shared_from_this()));
}

void denarius::network::async_listener::on_accept(beast::error_code ec, tcp::socket socket)
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
