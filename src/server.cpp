#include "include/server.h"

Server::Server(boost::asio::io_context &io_context, uint16_t port) :
        m_acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    DoAccept();
}

void Server::DoAccept() {
    m_acceptor_.async_accept([this](boost::system::error_code err_code, boost::asio::ip::tcp::socket socket) {
        Logger &logger = Logger::GetInstance();

        if (!err_code) {
            logger.log->info("Creating session on: " +
                             socket.remote_endpoint().address().to_string() + ":" +
                             std::to_string(socket.remote_endpoint().port()));

            std::make_shared<Session>(std::move(socket))->Run();
        } else {
            logger.log->info("Session on: " +
                             socket.remote_endpoint().address().to_string() + ":" +
                             std::to_string(socket.remote_endpoint().port()) + " - " +
                             err_code.message());
        }

        DoAccept();
    });
}