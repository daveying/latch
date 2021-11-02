#ifndef EVENT_QUEUE_HPP__
#define EVENT_QUEUE_HPP__

#include <map>
#include <queue>
#include <mutex>

#include <IScheduler.hpp>

namespace sched
{

class EventQueue
{
public:
    using EventQueueInternal = std::map<Timestamp, std::queue<Event>>;
    EventQueue(Timestamp initTime = 0);
    void addEvent(Timestamp relativeTime, Event event);
    bool getLatestEvent(Event& event);
    Timestamp getCurrentTimestamp() const;
    bool advanceTime();
#ifdef DEBUG
    const EventQueueInternal& getEventQueueInternal() const;
#endif
protected:
    EventQueueInternal m_events;
    Timestamp m_currTime;
    std::mutex m_qMutex;
};

} // namespace sched

#endif // EVENT_QUEUE_HPP__
