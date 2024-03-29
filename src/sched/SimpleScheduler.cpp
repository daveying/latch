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

#include <SimpleScheduler.hpp>

#include <iostream>

namespace sched
{

void SimpleScheduler::addEvent(Period relativeTime, Event event)
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
            std::cout << "[Sched][" << currTime << "] execute event >> \"" << latestEvent.name() << "\"" << std::endl;
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
