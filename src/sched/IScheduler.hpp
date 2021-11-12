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

    inline void operator()(Timestamp currTime)
    {
        this->m_execute(currTime);
    }
    inline const std::string& name() const
    {
        return m_name;
    }

    static Event create(const std::string& name, ExeHandle execute)
    {
        return Event{name, execute};
    }

    Event() {}

protected:
    Event(const std::string& name, ExeHandle execute)
        : m_name{name}
        , m_execute{execute}
    {
    }
    ExeHandle m_execute;
    std::string m_name;
};

class IScheduler
{
public:
    virtual void addEvent(Period relativeTime, Event event) = 0;
    virtual Timestamp getCurrentTimestamp() const = 0;
    virtual void run()  = 0;
    virtual void stop() = 0;
};

void addEvent(Period relativeTime, Event event);
void run();
void waitTillSteady();
Timestamp getCurrentTimestamp();

} // namespace sched

#endif // ISCHEDULER_HPP_
