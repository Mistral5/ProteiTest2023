#include "include/call_center.h"

CallCenter::CallCenter() {
    Start();
}

void CallCenter::Start() {
    ConfigReader &config = ConfigReader::GetInstance();

    Logger::Init(config);

    Logger &logger = Logger::GetInstance();

    CdrRequestQueue &cdr_queue = CdrRequestQueue::GetInstance();
    cdr_queue.set_max_size(config.get_queue_size());

    std::thread t1([&]() {
        boost::asio::io_context io_context;
        Server s(io_context, config.get_port());
        io_context.run();
    });

    std::thread t2([&]() {
        Operator oper;
    });

    t1.join();
    t2.join();
}