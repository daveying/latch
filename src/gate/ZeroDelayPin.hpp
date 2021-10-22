#ifndef Zero_DELAY_PIN_HPP_
#define Zero_DELAY_PIN_HPP_

#include <vector>

#include <IPin.hpp>
#include <IGate.hpp>
#include <IScheduler.hpp>
#include <iostream>

namespace gate
{

template <sched::Timestamp DELAY>
class DelayedInputPin : public IPin
{
public:
    static_assert(DELAY >= 0, "Input pin delay must >= 0");
    explicit DelayedInputPin(IGate* parent)
        : m_parent{parent}
        , m_value{PinState::Low}
    {}
    virtual const IGate* parent() const override
    {
        return m_parent;
    }
    virtual PinState value() const override
    {
        return m_value;
    }
    virtual void value(PinState newVal) override
    {
        if (m_value != newVal)
        {
            m_value = newVal;
            sched::addEvent(DELAY, [gate = m_parent] () -> void { gate->compute(); });
        }
    }
protected:
    IGate* m_parent;
    PinState m_value;
};

using ZeroDelayInputPin = DelayedInputPin<0>;

class ZeroDelayOutputPin : public ISourcePin
{
public:
    explicit ZeroDelayOutputPin(IGate* parent)
        : m_parent{parent}
        , m_value{PinState::Low}
    {}
    virtual const IGate* parent() const override
    {
        return m_parent;
    }
    virtual PinState value() const override
    {
        return m_value;
    }
    virtual void value(PinState newVal) override
    {
        if (m_value != newVal)
        {
            m_value = newVal;
            // zero delay
            for (auto peer : m_peers)
            {
                peer->value(newVal);
            }
        }
    }
    virtual void connect(IPin* pear) override
    {
        m_peers.push_back(pear);
        pear->value(m_value); // zero delay
    }
    virtual std::vector<IPin*>& peers() override
    {
        return m_peers;
    }
protected:
    IGate* m_parent;
    PinState m_value;
    std::vector<IPin*> m_peers;
};


} // namespace gate

#endif
