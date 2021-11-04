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
    virtual void value(PinState newVal) override
    {
        if (m_valueImmediate != newVal)
        {
            // IMPORTANT: the value of the pin will be set immediately
            // no matter how much the DELAY is, but there is a very subtle
            // case where the value of the pin will be set again before the
            // gate->compute() be called in the event even the DELAY is 0.
            // gete->compute() will always get the latest pin value.
            // so the right sequence is
            //    inputPin->value(High)
            //    gate->compute()          # compute with High
            //    inputPin->value(Low)
            //    gate->compute()          # re-compute with Low
            // but following can happen because the compute is pushed into
            // the event queue which may be executed after the second value
            // setting
            //    inputPin->value(High)
            //    inputPin->value(Low)
            //    gate->compute()          # compute with Low (wrong)
            //    gate->compute()          # compute with Low
            // for the edge detector circuit (tests/edge-detector) and D flip flop
            // circuit (tests/d-flip-flop), if the not gate(invertor) has zero delayed
            // input pin, this case can happen, so the edge detector need to have a
            // delayed input pin.
            // THIS IS NOT A BUG! because in real world, this kind of subtle timing
            // issue can also happen!!
            // Give this case a name for easy memorize: Reset before compute (RBC)
            m_valueImmediate = newVal;
            sched::addEvent(DELAY, sched::Event::create(
                        "Input pin trigger recompute",
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
    virtual void value(PinState newVal) override
    {
        if (m_value != newVal)
        {
            // output port value will be set immediately no matter the DEALY is
            m_value = newVal;
            sched::addEvent(DELAY, sched::Event::create(
                        "Output pin change forward value",
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
        sched::addEvent(DELAY, sched::Event::create(
                        "Output pin connect forward value",
                        [pin = this, peer = peer, value = m_value] (sched::Timestamp) -> void {
                            // peer will be forwarded with the value when the
                            // event is triggered
                            peer->value(value);
                        }
                    ));
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
