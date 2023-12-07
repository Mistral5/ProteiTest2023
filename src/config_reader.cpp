#include "include/config_reader.h"

ConfigReader *ConfigReader::p_instance_ = nullptr;
ConfigReaderDestroyer ConfigReader::destroyer_;

ConfigReaderDestroyer::~ConfigReaderDestroyer() {
    delete p_instance_;
}

void ConfigReaderDestroyer::Init(ConfigReader *p) {
    p_instance_ = p;
}

ConfigReader &ConfigReader::GetInstance() {
    if (!p_instance_) {
        p_instance_ = new ConfigReader();
        destroyer_.Init(p_instance_);
    }
    return *p_instance_;
}

ConfigReader::ConfigReader() {
    std::string file_content = ReadConfigFile();
    boost::json::object content_object;
    ParseFile(file_content, content_object);

    boost::json::object settings;

    try {
        settings = content_object["settings"].as_object();
    } catch (...) {
        std::cout << "Parsing failed, \"settings\" section not found in configuration file.\n";
        exit(EXIT_FAILURE);
    }

    SetServerParams(settings);
    SetQueueParams(settings);
    SetLoggerParams(settings);
}

std::string ConfigReader::ReadConfigFile() {
    std::ifstream file_stream;
    std::string file_content;

    file_stream.open(config_file_name_);
    if (file_stream.good()) {
        file_content = std::string(std::istreambuf_iterator<char>{file_stream}, {});
        file_stream.close();
        return file_content;
    }

    file_stream.open("../../../../" + config_file_name_);
    if (file_stream.good()) {
        file_content = std::string(std::istreambuf_iterator<char>{file_stream}, {});
        file_stream.close();
        return file_content;
    }

    std::cout << "Configuration file \"" << config_file_name_ << "\" was not found!\n";
    exit(EXIT_FAILURE);
}

void ConfigReader::ParseFile(std::string &file_content, boost::json::object &content_object) {
    boost::json::monotonic_resource mr;
    boost::json::parse_options opt;
    opt.allow_comments = true;
    std::error_code err_code;

    boost::json::value content_value = boost::json::parse(file_content, err_code, &mr, opt);

    if (err_code) {
        std::cout << "Parsing failed, " << err_code.message() << " in configuration file.\n";
        exit(EXIT_FAILURE);
    }

    content_object = content_value.as_object();
}

void ConfigReader::SetServerParams(boost::json::object &settings) {
    boost::json::object server;
    try {
        server = settings["server"].as_object();
    }
    catch (...) {
        std::cout << "Parsing failed, \"server\" section was not found in configuration file.\n";
        exit(EXIT_FAILURE);
    }

    config[Param::port] = CheckIntParam<uint16_t>(server, "port");
    config[Param::phone_num_format] = boost::regex(server["phone_num_format"].as_string().c_str());
    config[Param::oper_num] = CheckIntParam<uint32_t>(server, "operators_num");
    config[Param::call_duration_min] = CheckIntParam<uint32_t>(server, "call_duration_min");
    config[Param::call_duration_max] = CheckIntParam<uint32_t>(server, "call_duration_max");
}

void ConfigReader::SetQueueParams(boost::json::object &settings) {
    boost::json::object queue;
    try {
        queue = settings["queue"].as_object();
    } catch (...) {
        std::cout << "Parsing failed, \"queue\" section was not found in configuration file.\n";
        exit(EXIT_FAILURE);
    }

    config[Param::queue_size] = CheckIntParam<uint16_t>(queue, "queue_size");
    config[Param::rmin] = CheckIntParam<uint32_t>(queue, "rmin");
    config[Param::rmax] = CheckIntParam<uint32_t>(queue, "rmax");
    config[Param::r_average] = get_rmax() - get_rmin();
}

void ConfigReader::SetLoggerParams(boost::json::object &settings) {
    boost::json::object logger;

    try {
        logger = settings["logger"].as_object();
    } catch (...) {
        std::cout << "Parsing failed, \"logger\" section was not found in configuration file.\n";
        exit(EXIT_FAILURE);
    }

    config[Param::logger_name] = logger["logger_name"].as_string().c_str();
    config[Param::log_level] = logger["log_level"].as_string().c_str();
    config[Param::log_file_name] = logger["log_file_name"].as_string().c_str();
    config[Param::cdr_file_name] = logger["cdr_file_name"].as_string().c_str();
    config[Param::rotation_max_file_size] = CheckIntParam<uint32_t>(logger, "rotation_max_file_size");
    config[Param::rotation_max_files] = CheckIntParam<uint32_t>(logger, "rotation_max_files");
}

template<typename T>
T ConfigReader::CheckIntParam(boost::json::object settings, const char *param) {
    if (!settings[param].is_int64()) {
        std::cout << "Parsing failed, \"" << param << "\" is missing or is not a numeric type!\n";
        exit(EXIT_FAILURE);
    }

    int64_t tmp = settings[param].as_int64();

    if (tmp <= 0 || tmp >= std::numeric_limits<T>::max()) {
        std::cout << "Invalid " << "\"" << param << "\" value!\n";
        exit(EXIT_FAILURE);
    }

    return static_cast<T>(tmp);
}

// Server part
uint16_t ConfigReader::get_port() const {
    return std::get<uint16_t>(config.find(Param::port)->second);
}

boost::regex ConfigReader::get_phone_num_format() const {
    return std::get<boost::regex>(config.find(Param::phone_num_format)->second);
}

uint32_t ConfigReader::get_oper_num() const {
    return std::get<uint32_t>(config.find(Param::oper_num)->second);
}

uint32_t ConfigReader::get_call_duration_min() const {
    return std::get<uint32_t>(config.find(Param::call_duration_min)->second);
}

uint32_t ConfigReader::get_call_duration_max() const {
    return std::get<uint32_t>(config.find(Param::call_duration_max)->second);
}

// Queue part
uint16_t ConfigReader::get_queue_size() const {
    return std::get<uint16_t>(config.find(Param::queue_size)->second);
}

uint32_t ConfigReader::get_rmin() const {
    return std::get<uint32_t>(config.find(Param::rmin)->second);
}

uint32_t ConfigReader::get_rmax() const {
    return std::get<uint32_t>(config.find(Param::rmax)->second);
}

uint32_t ConfigReader::get_r_average() const {
    return std::get<uint32_t>(config.find(Param::r_average)->second);
}

// Logger part
std::string ConfigReader::get_logger_name() const {
    return std::get<std::string>(config.find(Param::logger_name)->second);
}

std::string ConfigReader::get_log_level() const {
    return std::get<std::string>(config.find(Param::log_level)->second);
}

std::string ConfigReader::get_log_file_name() const {
    return std::get<std::string>(config.find(Param::log_file_name)->second);
}

std::string ConfigReader::get_cdr_file_name() const {
    return std::get<std::string>(config.find(Param::cdr_file_name)->second);
}

uint32_t ConfigReader::get_rotation_max_file_size() const {
    return std::get<uint32_t>(config.find(Param::rotation_max_file_size)->second);
}

uint32_t ConfigReader::get_rotation_max_files() const {
    return std::get<uint32_t>(config.find(Param::rotation_max_files)->second);
}

//тут нужно написать несколько тестов и проставить логи