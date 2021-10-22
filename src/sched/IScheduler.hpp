#ifndef ISCHEDULER_HPP_
#define ISCHEDULER_HPP_

#include <functional>

namespace sched
{
using Event     = std::function<void()>;
using Timestamp = int64_t;
using Period    = int64_t;

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
