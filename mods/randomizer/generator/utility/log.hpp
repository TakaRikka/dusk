#pragma once

#include <string>
#include <fstream>
#include <list>

#include "../seedgen/config.hpp"
#include "../utility/path.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define __FILENAME__ (&__FILE__[SOURCE_PATH_SIZE])

namespace randomizer::utility::log
{
    class LogInfo
    {
       private:
        seedgen::config::Config config;
        std::string seedHash;

        LogInfo();
        ~LogInfo();

        static LogInfo& getInstance();

       public:
        LogInfo(const LogInfo&) = delete;
        LogInfo& operator=(const LogInfo&) = delete;

        static void setConfig(const seedgen::config::Config& config_) { getInstance().config = config_; }
        static void setSeedHash(const std::string& seedHash_) { getInstance().seedHash = seedHash_; }
        static const seedgen::config::Config& getConfig();
        static const std::string& getSeedHash();
    };

    class ErrorLog
    {
       private:
        static constexpr size_t MAX_ERRORS = 5;

        std::ofstream output;
        std::list<std::string> lastErrors;

        ErrorLog();
        ~ErrorLog();

       public:
        const fspath LOG_PATH = Utility::get_app_save_path() / "Error Log.txt";

        ErrorLog(const ErrorLog&) = delete;
        ErrorLog& operator=(const ErrorLog&) = delete;

        static ErrorLog& getInstance();
        void log(const std::string& msg, const bool& timestamp = true);
        std::string getLastErrors() const;
        void clearLastErrors();
    };

#define LOG_ERR_AND_RETURN(error)                                                                                             \
    {                                                                                                                         \
        ErrorLog::getInstance().log(std::string("Encountered " #error " on line " TOSTRING(__LINE__) " of ") + __FILENAME__); \
        return error;                                                                                                         \
    }

#define LOG_AND_RETURN_IF_ERR(func)                                                                                          \
    {                                                                                                                        \
        if (const auto error = func; error != decltype(error)::NONE)                                                         \
        {                                                                                                                    \
            ErrorLog::getInstance().log(std::string("Encountered error on line " TOSTRING(__LINE__) " of ") + __FILENAME__); \
            return error;                                                                                                    \
        }                                                                                                                    \
    }

#define LOG_ERR_AND_RETURN_BOOL(error)                                                                                        \
    {                                                                                                                         \
        ErrorLog::getInstance().log(std::string("Encountered " #error " on line " TOSTRING(__LINE__) " of ") + __FILENAME__); \
        return false;                                                                                                         \
    }

#define LOG_AND_RETURN_BOOL_IF_ERR(func)                                                                                     \
    {                                                                                                                        \
        if (const auto error = func; error != decltype(error)::NONE)                                                         \
        {                                                                                                                    \
            ErrorLog::getInstance().log(std::string("Encountered error on line " TOSTRING(__LINE__) " of ") + __FILENAME__); \
            return false;                                                                                                    \
        }                                                                                                                    \
    }

    class DebugLog
    {
       private:
        std::ofstream output;

        DebugLog();
        ~DebugLog();

       public:
        const fspath LOG_PATH = Utility::get_app_save_path() / "Debug Log.txt";

        DebugLog(const DebugLog&) = delete;
        DebugLog& operator=(const DebugLog&) = delete;

        static DebugLog& getInstance();
        void log(const std::string& msg, const bool& timestamp = true);
    };
} // namespace randomizer::utility::log

#ifdef RANDO_DEBUG
#define LOG_TO_DEBUG(message)                                                                                    \
    randomizer::utility::log::DebugLog::getInstance().log(std::string("Message on line " TOSTRING(__LINE__) " of ") + \
                                                     __FILENAME__ + std::string(": " + std::string(message)));
#else
#define LOG_TO_DEBUG(message)
#endif

#define LOG_TO_ERROR(message)                                                                                    \
    randomizer::utility::log::ErrorLog::getInstance().log(std::string("Message on line " TOSTRING(__LINE__) " of ") + \
                                                     __FILENAME__ + std::string(": " + std::string(message)));
