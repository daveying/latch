#ifndef Zero_DELAY_PIN_HPP_
#define Zero_DELAY_PIN_HPP_

#include <vector>

#include <IPin.hpp>
#include <IGate.hpp>
#include <IScheduler.hpp>
#include <Logger.hpp>

namespace gate
{

template <sched::Timestamp DELAY>
class DelayedInputPin : public IPin
{
public:
    static_assert(DELAY >= 0, "Input pin delay must >= 0");
    explicit DelayedInputPin(IGate* parent, int64_t index = -1)
        : m_parent{parent}
        , m_value{PinState::Low}
        , m_index{index}
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
            sched::addEvent(DELAY, sched::Event::create(
                        "Input pin trigger recompute",
                        [gate = m_parent] (sched::Timestamp) -> void { gate->compute(); },
                        [gate = m_parent, newVal = newVal, pin = this] (log::Logger& logger) {
                            logger << "Input pin [" << gate->name() << "." << pin->index() << "] delayed [" << DELAY << "] value [" << pinStateStr(newVal) << "] trigger gate [" << gate->name() << "] recompute";
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
    int64_t m_index;
};

using ZeroDelayInputPin = DelayedInputPin<0>;

class ZeroDelayOutputPin : public ISourcePin
{
public:
    explicit ZeroDelayOutputPin(IGate* parent, int64_t index = -1)
        : m_parent{parent}
        , m_value{PinState::Low}
        , m_index{index}
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


} // namespace gate

#endif
