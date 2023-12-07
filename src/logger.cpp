#include "include/logger.h"

Logger *Logger::p_instance_ = nullptr;


Logger &Logger::GetInstance() {
    if (!p_instance_) {
        std::cout << "The logger must be initialized before it can be called\n";
        exit(EXIT_FAILURE);
    }
    return *p_instance_;
}

Logger &Logger::Init(ConfigReader &config) {
    if (!p_instance_) {
        p_instance_ = new Logger(config);
    }
    return *p_instance_;
}

Logger::Logger(ConfigReader &config) {
    logger_name_ = config.get_logger_name();
    log_level_ = config.get_log_level();
    log_file_name_ = config.get_log_file_name();
    cdr_file_name_ = config.get_cdr_file_name();
    rotation_max_file_size_ = config.get_rotation_max_file_size() * 1024;
    rotation_max_files_ = config.get_rotation_max_files();

    InitLogSink();
    InitCdrSink();
}

Logger::~Logger() {
    delete p_instance_;
    log->info("Logging finish.\n");
    cdr->info("CDR recording finish.\n");
}

void Logger::InitLogSink() {
    try {
        log = spdlog::rotating_logger_mt<spdlog::async_factory>
                (logger_name_, log_file_name_, rotation_max_file_size_, rotation_max_files_);

        log->info("Start logging ...");
        log->set_level(spdlog::level::from_str(log_level_));
    }
    catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Logger init failed: " << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Logger::InitCdrSink() {
    try {
        cdr = spdlog::rotating_logger_mt<spdlog::async_factory>
                (cdr_file_name_, cdr_file_name_, rotation_max_file_size_, rotation_max_files_);

        cdr->info("Start CDR recording ...");
        cdr->set_level(spdlog::level::info);
        cdr->set_pattern("%v");
    }
    catch (const spdlog::spdlog_ex &ex) {
        std::cout << "CDR recorder init failed: " << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}