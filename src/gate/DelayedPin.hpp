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

#ifndef DELAYED_PIN_HPP_
#define DELAYED_PIN_HPP_

#include <vector>

#include <IPin.hpp>
#include <IGate.hpp>
#include <IScheduler.hpp>
#include <Logger.hpp>

namespace gate
{

template <sched::Period DELAY>
class DelayedInputPin : public IPin
{
public:
    static_assert(DELAY >= 0, "Input pin delay must >= 0");
    explicit DelayedInputPin(IGate* parent, int64_t index = -1)
        : m_parent{parent}
        , m_value{PinState::Low}
        , m_valueImmediate{PinState::Low}
        , m_index{index}
    {}
    virtual const component::IComponent* parent() const override
    {
        return m_parent;
    }
    virtual component::IComponent* parent() override
    {
        return m_parent;
    }
    virtual PinState value() const override
    {
        return m_value;
    }
    // compute the name every time to save memory
    // revisit to determine if name should be stored
    // in a member
    std::string name() const
    {
        return (m_parent ? m_parent->name() : "None") + ".in" + std::to_string(m_index);
    }
    virtual void value(PinState newVal) override
    {
        if (m_valueImmediate != newVal)
        {
            // m_valueImmediate is for record of pin value change, the new pin value will be
            // set until the schedule schedule it, so the new value will be captured by the
            // lambda and the pin value will be set when event is executed.
            m_valueImmediate = newVal;
            sched::addEvent(DELAY, sched::Event::create(
                        "Input pin  [" + name() + "] new value [" + pinStateStr(newVal) + "] trigger recompute",
                        [gate = m_parent, newVal = newVal, pin = this] (sched::Timestamp) -> void {
                            pin->m_value = newVal;
                            if(gate)
                            {
                                gate->compute();
                            }
                        }
                    ));
        }
    }
    virtual int64_t index() const override
    {
        return m_index;
    }
    virtual void index(int64_t idx) override
    {
        m_index = idx;
    }
protected:
    IGate* m_parent;
    PinState m_value;
    PinState m_valueImmediate;
    int64_t m_index;
};

using ZeroDelayInputPin = DelayedInputPin<0>;

template <sched::Period DELAY>
class DelayedOutputPin : public ISourcePin
{
public:
    explicit DelayedOutputPin(IGate* parent, int64_t index = -1)
        : m_parent{parent}
        , m_value{PinState::Low}
        , m_index{index}
    {}
    virtual const component::IComponent* parent() const override
    {
        return m_parent;
    }
    virtual component::IComponent* parent() override
    {
        return m_parent;
    }
    virtual PinState value() const override
    {
        return m_value;
    }
    // compute the name every time to save memory
    // revisit to determine if name should be stored
    // in a member
    std::string name() const
    {
        return (m_parent ? m_parent->name() : "None") + ".out" + std::to_string(m_index);
    }
    virtual void value(PinState newVal) override
    {
        // no matter if there is a different value, always forward to peer to
        // decide the response.
        // output port value will be set immediately no matter the DEALY is
        m_value = newVal;
        sched::addEvent(DELAY, sched::Event::create(
                    "Output pin [" + name() + "] change forward value [" + pinStateStr(newVal) + "]",
                    [newVal = newVal, pin = this] (sched::Timestamp) -> void {
                        for (auto peer : pin->m_peers)
                        {
                            // peers will be forwarded with the value when the
                            // event is triggered
                            peer->value(newVal);
                        }
                    }
                ));
    }
    virtual void connect(IPin* peer) override
    {
        m_peers.push_back(peer);
    }
    virtual std::vector<IPin*>& peers() override
    {
        return m_peers;
    }
    virtual int64_t index() const override
    {
        return m_index;
    }
    virtual void index(int64_t idx) override
    {
        m_index = idx;
    }
protected:
    IGate* m_parent;
    PinState m_value;
    std::vector<IPin*> m_peers;
    int64_t m_index;
};

using ZeroDelayOutputPin = DelayedOutputPin<0>;

} // namespace gate

#endif
