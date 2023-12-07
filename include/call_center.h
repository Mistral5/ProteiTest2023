#ifndef INCLUDE_CALL_CENTER_H_
#define INCLUDE_CALL_CENTER_H_

#include "cdr_request_queue.h"
#include "cdr_request.h"
#include "cdr.h"
#include "server.h"
#include "config_reader.h"
#include "logger.h"
#include "operator.h"

#include <thread>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <spdlog/spdlog.h>
#include <queue>
#include <chrono>

class CallCenter {
public:
    CallCenter();

private:
    void Start();
};

#endif // INCLUDE_CALL_CENTER_H_