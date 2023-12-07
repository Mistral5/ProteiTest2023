#ifndef INCLUDE_CONFIG_READER_H_
#define INCLUDE_CONFIG_READER_H_

#include <fstream>
#include <cstdint>
#include <chrono>
#include <string>
#include <iostream>
#include <iterator>
#include <thread>
#include <limits>

#include <boost/json.hpp>
#include <boost/regex.hpp>

class ConfigReader;

class ConfigReaderDestroyer {
private:
    ConfigReader *p_instance_;
public:
    ~ConfigReaderDestroyer();

    void Init(ConfigReader *);
};

class ConfigReader {
private:
    enum class Param {
        //Server part
        port,
        phone_num_format,
        oper_num,
        call_duration_min,
        call_duration_max,
        //Queue part
        queue_size,
        rmin,
        rmax,
        r_average,
        //Logger part
        logger_name,
        log_level,
        log_file_name,
        cdr_file_name,
        rotation_max_file_size,
        rotation_max_files
    };

public:
    static ConfigReader &GetInstance();

    ConfigReader(const ConfigReader &) = delete;

    ConfigReader &operator=(ConfigReader &) = delete;

    // Server part
    uint16_t get_port() const;

    boost::regex get_phone_num_format() const;

    uint32_t get_oper_num() const;

    uint32_t get_call_duration_min() const;

    uint32_t get_call_duration_max() const;

    // Queue part
    uint16_t get_queue_size() const;

    uint32_t get_rmin() const;

    uint32_t get_rmax() const;

    uint32_t get_r_average() const;

    // Logger part
    std::string get_logger_name() const;

    std::string get_log_level() const;

    std::string get_log_file_name() const;

    std::string get_cdr_file_name() const;

    uint32_t get_rotation_max_file_size() const;

    uint32_t get_rotation_max_files() const;

private:
    static ConfigReader *p_instance_;
    static ConfigReaderDestroyer destroyer_;

    ConfigReader();

    ~ConfigReader() {};

    friend class ConfigReaderDestroyer;

    std::string ReadConfigFile();

    void ParseFile(std::string &, boost::json::object &);

    void SetServerParams(boost::json::object &);

    void SetQueueParams(boost::json::object &);

    void SetLoggerParams(boost::json::object &);

    template<typename T>
    T CheckIntParam(boost::json::object, const char *);

    std::map<Param, std::variant<uint16_t, uint32_t, std::string, boost::regex>> config;
    std::string config_file_name_ = "config.json";
};

#endif // INCLUDE_CONFIG_READER_H_