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

#include <Timer.hpp>

namespace component
{

Timer::Timer(sched::Period period, const std::string& name)
    : m_period{period}
    , m_pin{nullptr}
    , m_value{gate::PinState::Low}
    , m_enabled{gate::PinState::Low}
    , m_halt{false}
    , m_name{name}
{
}

void Timer::initialize()
{
    sched::addEvent(m_period, sched::Event::create(
                "Timer flip",
                [self = this] (sched::Timestamp) {
                    self->timerEvent();
                }
            ));
}

const std::string& Timer::name() const
{
    return m_name;
}

gate::IPin* Timer::pin(size_t idx)
{
    throw std::runtime_error("Timer::pin(): Invalid operation");
}

void Timer::halt()
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_halt = true;
}

void Timer::connect(gate::IPin* pin)
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_pin = pin;
}

void Timer::disconnect()
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_pin = nullptr;
}

void Timer::timerEvent()
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    if (m_halt)
    {
        return;
    }
    if (m_enabled == gate::PinState::High)
    {
        m_value = ~m_value;
    }
    if (m_pin != nullptr)
    {
        m_pin->value(m_value);
    }
    // NOTE: will not dead lock because scheduler will execute
    // newly added event after this event returns, even the
    // m_period is 0.
    sched::addEvent(m_period, sched::Event::create(
                "Timer flip",
                [self = this] (sched::Timestamp) {
                    self->timerEvent();
                }
            ));
}

void Timer::enable(gate::PinState value)
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_enabled = value;
}

} // namespace component
