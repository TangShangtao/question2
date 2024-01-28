#pragma once

#include <string>

enum DirectionType
{
    Buy = 0,
    Sell = 1
};

struct DailyData
{
    std::string date;
    double open;
    double high;
    double low;
    double close;
};

struct Order
{
    uint32_t orderID;
    std::string date;
    DirectionType direction;
    double price;
    double volume;
    int signal;
    bool finished = false;
};

struct Trade
{
    uint32_t orderID;
    std::string date;
    DirectionType direction;
    double price;
    double volume;
    double fees;
    int signal;
};

class MDSubscriber
{
public:
    virtual void OnBacktestInit() = 0;

    virtual void OnBacktestStart() = 0;
    virtual void OnBacktestEnd() = 0;
    
    virtual void OnMDUpdate(const DailyData& marketData) = 0;
    virtual ~MDSubscriber() {}
};

class TradeSubscriber
{
public:
    virtual void OnRtnTrade(const Trade& trade) = 0;
    virtual ~TradeSubscriber() {};
};


#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#define TRACE(...) SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__)
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#define WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#define ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)
#define CRITICAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__)

// #define TRACE(...) spdlog::trace(__VA_ARGS__)
// #define DEBUG(...) spdlog::debug(__VA_ARGS__)
// #define INFO(...) spdlog::info(__VA_ARGS__)
// #define WARN(...) spdlog::warn(__VA_ARGS__)
// #define ERROR(...) spdlog::error(__VA_ARGS__)
// #define CRITICAL(...) spdlog::critical(__VA_ARGS__)

#define LEVEL_TRACE 0
#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4
#define LEVEL_CRITICAL 5
#define LEVEL_OFF 6

class Logger
{
public:
    enum level_enum
    {
        trace = LEVEL_TRACE,
        debug = LEVEL_DEBUG,
        info = LEVEL_INFO,
        warn = LEVEL_WARN,
        err = LEVEL_ERROR,
        critical = LEVEL_CRITICAL,
        off = LEVEL_OFF,
        n_levels
    };

    static void Init(level_enum level)
    {
    spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::set_pattern("%L[%T.%f][%P][%t][%s:%#] %v");
    auto console = spdlog::stdout_color_mt("console");
    spdlog::stderr_color_mt("stderr");
    // spdlog::set_default_logger(console);

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm = localtime(&now);
    char filename[60] = { 0 };
    sprintf(filename, "logs/run-%d-%02d-%02d.log", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday);
    // Create a file rotating logger with 100mb size max and 30 rotated files
    auto max_size = 1024 * 1024 * 300;
    auto max_files = 10;
    spdlog::rotating_logger_mt("Service", filename, max_size, max_files);

//    spdlog::daily_logger_mt("Service", "logs/run.log", 0, 00);

    spdlog::set_default_logger(spdlog::get("Service"));
    }
};