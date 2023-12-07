#ifndef INCLUDE_SERVER_H_
#define INCLUDE_SERVER_H_

#include "include/session.h"

#include <iostream>
#include <cinttypes>

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

class Server {
public:
    Server(boost::asio::io_context &, uint16_t);

private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor m_acceptor_;
};

#endif // INCLUDE_SERVER_H_