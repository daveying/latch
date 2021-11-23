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

#ifndef SIMPLE_SCHEDULER_HPP__
#define SIMPLE_SCHEDULER_HPP__

#include <IScheduler.hpp>
#include <EventQueue.hpp>

namespace sched
{

class SimpleScheduler : public IScheduler
{
public:
    virtual void addEvent(Period relativeTime, Event event) override;
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
