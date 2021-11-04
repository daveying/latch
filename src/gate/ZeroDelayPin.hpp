#ifndef Zero_DELAY_PIN_HPP_
#define Zero_DELAY_PIN_HPP_

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
    virtual const IGate* parent() const override
    {
        return m_parent;
    }
    virtual PinState value() const override
    {
        return m_value;
    }
    std::string name() const
    {
        return m_parent->name() + "." + std::to_string(m_index);
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
                        "Input pin[" + name() + "] trigger recompute",
                        [gate = m_parent, newVal = newVal, pin = this] (sched::Timestamp) -> void {
                            pin->m_value = newVal;
                            gate->compute();
                        },
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
    virtual const IGate* parent() const override
    {
        return m_parent;
    }
    virtual PinState value() const override
    {
        return m_value;
    }
    std::string name() const
    {
        return (m_parent ? m_parent->name() : "None") + "." + std::to_string(m_index);
    }
    virtual void value(PinState newVal) override
    {
        if (m_value != newVal)
        {
            // output port value will be set immediately no matter the DEALY is
            m_value = newVal;
            sched::addEvent(DELAY, sched::Event::create(
                        "Output pin[" + name() + "] change forward value",
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
