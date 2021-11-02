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

Timestamp getCurrentTimestamp()
{
    return scheduler.getCurrentTimestamp();
}

#ifdef DEBUG
const EventQueue::EventQueueInternal& getEventQueueInternal()
{
    return scheduler.getEventQueueInternal();
}
#endif
} // namespace sched
