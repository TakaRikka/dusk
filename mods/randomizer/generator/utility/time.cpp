#include "../utility/time.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

namespace randomizer::utility::time
{
    ProgramTime::ProgramTime(): openTime(Clock_t::now()) {}

    ProgramTime& ProgramTime::getInstance()
    {
        static ProgramTime s_Instance;
        return s_Instance;
    }

    ProgramTime::TimePoint_t ProgramTime::getOpenedTime()
    {
        return getInstance().openTime;
    }

    ProgramTime::Duration_t ProgramTime::getElapsedTime()
    {
        return Clock_t::now() - getOpenedTime();
    }
} // namespace randomizer::utility::time
#if __has_include(<format>) && !defined(__APPLE__)
#include <format>
namespace randomizer::utility::time
{
    std::string ProgramTime::getDateStr()
    {
        return std::format("{0:%a, %b %d, %Y, %I:%M:%S %p%n}", round<seconds>(getOpenedTime()));
    }

    std::string ProgramTime::getTimeStr()
    {
        return std::format("{:%T}", round<milliseconds>(getElapsedTime()));
    }
} // namespace randomizer::utility::time
#else
#include <sstream>
#include <mutex>
namespace randomizer::utility::time
{
    std::string ProgramTime::getDateStr()
    {
        const time_t point = Clock_t::to_time_t(ProgramTime::getOpenedTime());

        static std::mutex localtimeMut; // std::ctime is not thread safe
        std::unique_lock<std::mutex> lock(localtimeMut);
        return std::ctime(&point); // time string ends with \n
    }

    std::string ProgramTime::getTimeStr()
    {
        Duration_t duration = getElapsedTime();
        std::stringstream ret;
        ret << std::setfill('0');

        const hours hr = duration_cast<hours>(duration);
        ret << std::setw(2) << hr.count() << ":";
        duration -= hr;
        const minutes min = duration_cast<minutes>(duration);
        ret << std::setw(2) << min.count() << ":";
        duration -= min;
        const seconds sec = duration_cast<seconds>(duration);
        ret << std::setw(2) << sec.count() << ".";
        duration -= sec;
        const milliseconds ms = duration_cast<milliseconds>(duration);
        ret << std::setw(3) << ms.count();

        return ret.str();
    }
} // namespace randomizer::utility::time
#endif
namespace randomizer::utility::time
{
    static const ProgramTime& temp = ProgramTime::getInstance(); // inaccessible global to create instance when program starts
}; // namespace randomizer::utility::time
