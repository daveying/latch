#ifndef ISCHEDULER_HPP_
#define ISCHEDULER_HPP_

#include <functional>
#include <string>
#include <Logger.hpp>

namespace sched
{

using Timestamp = int64_t;
using Period    = int64_t;

class Event
{
public:
    using ExeHandle = std::function<void(Timestamp)>;
    using LogHandle = std::function<void(log::Logger&)>;

    inline void operator()(Timestamp currTime)
    {
        this->m_execute(currTime);
    }
    inline void log(log::Logger& logger)
    {
        if (m_log != nullptr)
        {
            this->m_log(logger);
        }
    }
    inline const std::string& name() const
    {
        return m_name;
    }

    static Event create(const std::string& name, ExeHandle execute, LogHandle log = nullptr)
    {
        return Event{name, execute, log};
    }

    Event() {}

protected:
    Event(const std::string& name, ExeHandle execute, LogHandle log)
        : m_name{name}
        , m_execute{execute}
        , m_log{log}
    {
    }
    ExeHandle m_execute;
    LogHandle m_log;
    std::string m_name;
};

class IScheduler
{
public:
    virtual void addEvent(Timestamp relativeTime, Event event) = 0;
    virtual Timestamp getCurrentTimestamp() const = 0;
    virtual void run()  = 0;
    virtual void stop() = 0;
};

void addEvent(Timestamp relativeTime, Event event);
void run();
void waitTillSteady();
Timestamp getCurrentTimestamp();

} // namespace sched

#endif // ISCHEDULER_HPP_
