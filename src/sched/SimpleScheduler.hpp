#ifndef SIMPLE_SCHEDULER_HPP__
#define SIMPLE_SCHEDULER_HPP__

#include <IScheduler.hpp>
#include <EventQueue.hpp>

namespace sched
{

class SimpleScheduler : public IScheduler
{
public:
    virtual void addEvent(Timestamp relativeTime, Event event) override;
    virtual Timestamp getCurrentTimestamp() const override;
    virtual void run() override;
    virtual void stop() override;
#ifdef DEBUG
    const EventQueue::EventQueueInternal& getEventQueueInternal() const;
#endif
protected:
    EventQueue m_queue;
};

} // namespace sched

#endif // SIMPLE_SCHEDULER_HPP__
