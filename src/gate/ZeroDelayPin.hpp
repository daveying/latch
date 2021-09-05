#ifndef ZERO_DELAY_PIN_HPP_
#define ZERO_DELAY_PIN_HPP_

#include <vector>

#include <IPin.hpp>
#include <IGate.hpp>
#include <IScheduler.hpp>

namespace gate
{

class ZeroDelayInputPin : public IPin
{
public:
    explicit ZeroDelayInputPin(IGate* parent)
        : m_parent(parent)
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
            sched::addEvent(0, [gate = m_parent] () -> void { gate->compute(); });
        }
    }
protected:
    IGate* m_parent;
    PinState m_value;
};

class ZeroDelayOutputPin : public ISourcePin
{
public:
    explicit ZeroDelayOutputPin(IGate* parent)
        : m_parent(parent)
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
