include(FetchContent)

set(BOOST_ENABLE_CMAKE ON)

set(
    BOOST_INCLUDE_LIBRARIES
    system
    asio
    date_time
    json
    regex
)

Set(FETCHCONTENT_QUIET FALSE)
message(STATUS "Downloading and extracting boost library sources. This will take some time... (about 5 minutes)")
FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.7z
    USES_TERMINAL_DOWNLOAD TRUE 
    GIT_PROGRESS TRUE
    DOWNLOAD_NO_EXTRACT FALSE
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

FetchContent_MakeAvailable(
    Boost
    spdlog
)

set(
    SRC_LIST 
    main.cpp
    call_center.cpp
    cdr.cpp
    cdr_request.cpp
    cdr_request_queue.cpp
    config_reader.cpp
    logger.cpp
    operator.cpp
    server.cpp
    session.cpp
)

add_executable (${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE 
    Boost::asio
    Boost::date_time
    Boost::json
    Boost::system
    spdlog::spdlog_header_only
)
