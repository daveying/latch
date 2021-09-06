#include <IScheduler.hpp>
#include <SimpleScheduler.hpp>

namespace sched
{

static SimpleScheduler scheduler;

// void addEvent(Timestamp relativeTime, Event event)
// {
//     event();
// }
void addEvent(Timestamp relativeTime, Event event)
{
    scheduler.addEvent(relativeTime, event);
}

void run()
{
    scheduler.run();
}

void waitTillSteady()
{
    run();
}
} // namespace sched
