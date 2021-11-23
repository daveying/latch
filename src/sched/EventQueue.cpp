/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2021 Xingpeng Da
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

#include <EventQueue.hpp>

namespace sched
{

EventQueue::EventQueue(Timestamp initTime)
    : m_currTime{initTime}
{
    // m_events always non-empty
    std::queue<Event> q;
    m_events.insert({m_currTime, q});
}

void EventQueue::addEvent(Period relativeTime, Event event)
{
    auto targetTime = m_currTime + relativeTime;
    std::unique_lock<std::mutex> lk(m_qMutex);
    auto found = m_events.find(targetTime);
    if (found != m_events.end())
    {
        found->second.push(event);
    }
    else
    {
        std::queue<Event> q;
        q.push(event);
        m_events.insert({targetTime, q});
    }
}

Timestamp EventQueue::getCurrentTimestamp() const
{
    return m_currTime;
}

bool EventQueue::getLatestEvent(Event& event)
{
    bool ret{false};
    std::unique_lock<std::mutex> lk(m_qMutex);
    auto& currQ = m_events.begin()->second;
    if (currQ.empty())
    {
        ret = false;
    }
    else
    {
        event = currQ.front();
        currQ.pop();
        ret = true;
    }
    return ret;
}

bool EventQueue::advanceTime()
{
    bool ret{false};
    std::unique_lock<std::mutex> lk(m_qMutex);
    auto curr = m_events.begin();
    if (!curr->second.empty())
    {
        ret = false; // current queue has event
    }
    else
    {
        // advance time
        if (++curr == m_events.end())
        {
            ret = false; // there is no other queue
        }
        else
        {
            ret = true;
            m_events.erase(m_events.begin());
            m_currTime = curr->first;
        }
    }
    return ret;
}

#ifdef DEBUG
const EventQueue::EventQueueInternal& EventQueue::getEventQueueInternal() const
{
    return m_events;
}
#endif

} // namespace sched
