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

#ifndef INERTIAL_PIN_HPP__
#define INERTIAL_PIN_HPP__

#include <DelayedPin.hpp>

namespace component
{

template <sched::Period DELAY>
class InertialInputPin : public DelayedInputPin<DELAY>
{
public:
    static std::string Name()
    {
        return "InertialInputPin_" + std::to_string(DELAY);
    }
    static_assert(DELAY >= 1, "Inertial has to bigger than one time unit to make sense");
    explicit InertialInputPin(IGate* parent, int64_t index = -1)
        : DelayedInputPin<DELAY>(parent, index)
    {}
    virtual void value(PinState newVal) override
    {
        if (this->m_valueImmediate != newVal)
        {
            this->m_valueImmediate = newVal;
            checkValue(newVal, DELAY);
        }
    }
    virtual ~InertialInputPin() override {}
private:
    void checkValue(PinState newVal, sched::Period timeLeft)
    {
        if (newVal == this->m_valueImmediate) // the value still hold
        {
            if (timeLeft == 0)
            {
                if (this->m_value != newVal)
                {
                    this->m_value = newVal;
                    if (this->m_parent) this->m_parent->compute();
                }
            }
            else
            {
                sched::addEvent(1, sched::Event::create(
                    "Input pin [" + this->name() + "] check value [" + pinStateStr(newVal) + "] at time [" + std::to_string(timeLeft - 1) + "] may trigger recompute",
                    [pin = this, newVal = newVal, timeLeft = (timeLeft - 1)] (sched::Timestamp) -> void {
                        pin->checkValue(newVal, timeLeft);
                    }
                ));
            }
        }
    }

};

using UnitInertialInputPin = InertialInputPin<1>;

} // namespace component

#endif // INERTIAL_PIN_HPP__
