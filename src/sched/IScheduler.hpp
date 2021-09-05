#ifndef ISCHEDULER_HPP_
#define ISCHEDULER_HPP_

#include <functional>

namespace sched
{
using Event     = std::function<void()>;
using Timestamp = uint64_t;

class IScheduler
{
public:
    virtual void addEvent(Timestamp relativeTime, Event event) = 0;
    virtual Timestamp getCurrentTimestamp() const = 0;
    virtual void run()  = 0;
    virtual void stop() = 0;
};

void addEvent(Timestamp relativeTime, Event event);

} // namespace sched

#endif // ISCHEDULER_HPP_
