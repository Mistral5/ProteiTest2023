#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "config_reader.h"
#include "spdlog/fmt/ostr.h"

#include <include/cdr.h>

class Logger {
public:
    static Logger &Init(ConfigReader &);

    static Logger &GetInstance();

    Logger(const Logger &) = delete;

    Logger &operator=(Logger &) = delete;

    ~Logger();

    std::shared_ptr<spdlog::logger> cdr;
    std::shared_ptr<spdlog::logger> log;

private:
    static Logger *p_instance_;

    Logger(ConfigReader &);

    void InitLogSink();

    void InitCdrSink();

    std::string logger_name_;
    std::string log_level_;
    std::string log_file_name_;
    std::string cdr_file_name_;
    uint32_t rotation_max_file_size_;
    uint32_t rotation_max_files_;
};

#endif // INCLUDE_LOGGER_H_