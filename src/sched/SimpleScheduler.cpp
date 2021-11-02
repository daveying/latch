#include <SimpleScheduler.hpp>

#include <iostream>

namespace sched
{

void SimpleScheduler::addEvent(Timestamp relativeTime, Event event)
{
    m_queue.addEvent(relativeTime, event);
}

Timestamp SimpleScheduler::getCurrentTimestamp() const
{
    return m_queue.getCurrentTimestamp();
}

void SimpleScheduler::run()
{
    Event latestEvent;
    do
    {
        Timestamp currTime = m_queue.getCurrentTimestamp();
        std::cout << "[Sched] execute timestamp: " << currTime << std::endl;
        while (m_queue.getLatestEvent(latestEvent))
        {
            std::cout << "[Sched][" << currTime << "] execute event [" << latestEvent.name() << "]\n  > ";
            latestEvent.log(std::cout);
            std::cout << std::endl;
            latestEvent(currTime);
        }
    } while (m_queue.advanceTime());
}

void SimpleScheduler::stop()
{
}

#ifdef DEBUG
const EventQueue::EventQueueInternal& SimpleScheduler::getEventQueueInternal() const
{
    return m_queue.getEventQueueInternal();
}
#endif

} // namespace sched
