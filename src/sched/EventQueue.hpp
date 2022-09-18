/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2022 Xingpeng Da
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/////////////////////////////////////////////////////////////////////////////////

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
    void addEvent(Period relativeTime, Event event);
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
