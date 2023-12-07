#include "include/operator.h"

Operator::Operator() {
    Start();
}

void Operator::Start() {
    CdrRequestQueue &cdr_queue = CdrRequestQueue::GetInstance();
    ConfigReader &config = ConfigReader::GetInstance();
    Logger &logger = Logger::GetInstance();

    for (;;) {
        if (!cdr_queue.empty()) {
            CdrRequest cdr_request = cdr_queue.front();
            cdr_queue.pop_front();

            boost::posix_time::ptime response_dt = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration diff = response_dt - cdr_request.get_start_dt();

            if (diff.total_milliseconds() > config.get_r_average()) {
                Cdr cdr(cdr_request, boost::posix_time::microsec_clock::local_time(), "Timeout");
                logger.cdr->info(cdr);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(
                        rand() % (config.get_call_duration_max() - config.get_call_duration_min()) +
                        config.get_call_duration_min()
                ));
                Cdr cdr(cdr_request, boost::posix_time::microsec_clock::local_time(), "OK", response_dt, 1);
                logger.cdr->info(cdr);
            }
        }
    }
}