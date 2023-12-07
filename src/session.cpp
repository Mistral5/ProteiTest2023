#include "include/session.h"

Session::Session(boost::asio::ip::tcp::socket socket)
        : m_socket_(std::move(socket)), call_quantity_(1) {}

void Session::Run() {
    WaitForRequest();
}

void Session::WaitForRequest() {
    auto self(shared_from_this());

    boost::asio::async_read_until(m_socket_, m_buffer_, "\0",
                                  [this, self](boost::system::error_code err_code, std::size_t) {
                                      Logger &logger = Logger::GetInstance();

                                      if (!err_code) {
                                          std::string request{std::istreambuf_iterator<char>(&m_buffer_), {}};
                                          m_buffer_.consume(m_buffer_.size());

                                          if (RequestIsValid(request)) {
                                              CdrRequestQueue &cdr_queue = CdrRequestQueue::GetInstance();

                                              CdrRequest cdr_request(
                                                      boost::posix_time::microsec_clock::local_time(),
                                                      call_quantity_,
                                                      boost::lexical_cast<uint64_t>(request)
                                              );

                                              if (!cdr_queue.Contains(cdr_request)) {
                                                  if (cdr_queue.push_back(cdr_request)) {
                                                      boost::asio::write(m_socket_, boost::asio::buffer(
                                                              std::to_string(call_quantity_)));
                                                  } else {
                                                      boost::asio::write(m_socket_, boost::asio::buffer("Overload!"));
                                                      Cdr cdr(cdr_request,
                                                              boost::posix_time::microsec_clock::local_time(),
                                                              "Overload");
                                                      logger.cdr->info(cdr);
                                                  }
                                                  ++call_quantity_;
                                              } else {
                                                  boost::asio::write(m_socket_,
                                                                     boost::asio::buffer("Error! Already in queue!"));
                                              }
                                          }
                                          WaitForRequest();
                                      } else {
                                          logger.log->info("Session on: " +
                                                           m_socket_.remote_endpoint().address().to_string() + ":" +
                                                           std::to_string(m_socket_.remote_endpoint().port()) + " - " +
                                                           err_code.message());
                                      }
                                  });
}

bool Session::RequestIsValid(std::string request) {
    ConfigReader &config = ConfigReader::GetInstance();
    Logger &logger = Logger::GetInstance();

    if (!boost::regex_match(request, config.get_phone_num_format())) {
        logger.log->info("Invalid request \"" + request + "\"");
        return false;
    }

    return true;
}