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

#ifndef ISCHEDULER_HPP_
#define ISCHEDULER_HPP_

#include <functional>
#include <string>
#include <Logger.hpp>

namespace sched
{

using Timestamp = int64_t;
using Period    = int64_t;

class Event
{
public:
    using ExeHandle = std::function<void(Timestamp)>;

    inline void operator()(Timestamp currTime)
    {
        this->m_execute(currTime);
    }
    inline const std::string& name() const
    {
        return m_name;
    }

    static Event create(const std::string& name, ExeHandle execute)
    {
        return Event{name, execute};
    }

    Event() {}

protected:
    Event(const std::string& name, ExeHandle execute)
        : m_name{name}
        , m_execute{execute}
    {
    }
    ExeHandle m_execute;
    std::string m_name;
};

class IScheduler
{
public:
    virtual void addEvent(Period relativeTime, Event event) = 0;
    virtual Timestamp getCurrentTimestamp() const = 0;
    virtual void run()  = 0;
    virtual void stop() = 0;
};

void addEvent(Period relativeTime, Event event);
void run();
void waitTillSteady();
Timestamp getCurrentTimestamp();

} // namespace sched

#endif // ISCHEDULER_HPP_
