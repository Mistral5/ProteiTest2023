#ifndef INCLUDE_SESSION_H_
#define INCLUDE_SESSION_H_

#include <include/cdr_request_queue.h>
#include <include/config_reader.h>
#include <include/logger.h>
#include <include/cdr.h>

#include <iostream>
#include <cinttypes>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket);

    void Run();

private:
    void WaitForRequest();

    bool RequestIsValid(std::string);

    boost::asio::ip::tcp::socket m_socket_;
    boost::asio::streambuf m_buffer_;
    std::atomic<uint32_t> call_quantity_;
};

#endif // INCLUDE_SESSION_H_