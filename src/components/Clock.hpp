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

#ifndef TIMER_HPP__
#define TIMER_HPP__

#include <mutex>
#include <IComponent.hpp>

namespace component
{

class Clock : public IEventComponent
{
public:
    Clock(sched::Period period, const std::string& name = "");
    virtual void initialize() override;
    virtual const std::string& name() const override;
    virtual IPin* pin(size_t idx) override;
    virtual void connect(IPin* pin) override;
    virtual void disconnect() override;
    virtual void enable(PinState value) override;
    void halt();
    virtual ~Clock() {}
protected:
    void timerEvent();

    sched::Period m_period;
    IPin* m_pin;
    PinState m_enabled;
    PinState m_value;
    bool m_halt;
    // the public APIs will be called in different threads
    std::mutex m_eventMutex;
    std::string m_name;
};

} // namespace component

#endif // TIMER_HPP__
