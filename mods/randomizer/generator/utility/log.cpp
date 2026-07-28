#include "../utility/log.hpp"
#include "../utility/time.hpp"

#define RANDOMIZER_VERSION "1.0.0"

namespace randomizer::utility::log
{
    LogInfo::LogInfo() {}

    LogInfo::~LogInfo() {}

    LogInfo& LogInfo::getInstance()
    {
        static LogInfo s_Instance;
        return s_Instance;
    }

    const randomizer::seedgen::config::Config& LogInfo::getConfig()
    {
        return getInstance().config;
    }

    const std::string& LogInfo::getSeedHash()
    {
        return getInstance().seedHash;
    }

    ErrorLog::ErrorLog()
    {
#ifdef RANDO_ERROR_LOG
        output.open(LOG_PATH);
        output << "Program opened " << randomizer::utility::time::ProgramTime::getDateStr(); // time string ends with \n
        output << "Dusk Randomizer Version " << RANDOMIZER_VERSION << std::endl;
        output << std::endl << std::endl;
#endif
    }

    ErrorLog::~ErrorLog()
    {
#ifdef RANDO_ERROR_LOG
        output.close();
#endif
    }

    ErrorLog& ErrorLog::getInstance()
    {
        static ErrorLog s_Instance;
        return s_Instance;
    }

    void ErrorLog::log(const std::string& msg, const bool& timestamp)
    {
#ifdef RANDO_ERROR_LOG
        if (timestamp)
            output << "[" << randomizer::utility::time::ProgramTime::getTimeStr() << "] ";
        output << msg << std::endl;
#endif
        lastErrors.push_front(msg);
    }

    std::string ErrorLog::getLastErrors() const
    {
        std::string retStr = "";
        for (auto& error : lastErrors)
        {
            retStr += error + "\n";
        }
        return retStr;
    }

    void ErrorLog::clearLastErrors()
    {
        lastErrors.clear();
    }

    DebugLog::DebugLog()
    {
        output.open(LOG_PATH);
        output << "Program opened " << randomizer::utility::time::ProgramTime::getDateStr(); // time string ends with \n
        output << "Dusk Randomizer Version " << RANDOMIZER_VERSION << std::endl;
        output << std::endl << std::endl;
    }

    DebugLog::~DebugLog()
    {
        output.close();
    }

    DebugLog& DebugLog::getInstance()
    {
        static DebugLog s_Instance;
        return s_Instance;
    }

    void DebugLog::log(const std::string& msg, const bool& timestamp)
    {
        if (timestamp)
            output << "[" << randomizer::utility::time::ProgramTime::getTimeStr() << "] ";
        output << msg << std::endl;
    }
} // namespace randomizer::utility::log
