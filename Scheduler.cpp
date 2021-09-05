#include <IScheduler.hpp>

namespace sched
{
void addEvent(Timestamp relativeTime, Event event)
{
    event();
}
} // namespace sched
