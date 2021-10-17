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
        std::cout << "Sched: execute timestamp: " << m_queue.getCurrentTimestamp() << std::endl;
        while (m_queue.getLatestEvent(latestEvent))
        {
            latestEvent();
        }
    } while (m_queue.advanceTime());
}

void SimpleScheduler::stop()
{
}

} // namespace sched
