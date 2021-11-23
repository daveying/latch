#ifndef FORWARD_PIN_HPP__ 
#define FORWARD_PIN_HPP__ 

#include <IPin.hpp>
#include <IComponent.hpp>
#include <ComponentDescription.hpp>

namespace component
{

template <PinDirection Direction>
class ForwardPin : public gate::ISourcePin
{
public:
    static constexpr auto direction()
    {
        return Direction;
    }

    explicit ForwardPin(IComponent* parent, int64_t index = -1)
        : m_parent{parent}
        , m_value{gate::PinState::Low}
        , m_index{index}
    {
    }

    virtual const IComponent* parent() const override
    {
        return m_parent;
    }
    virtual IComponent* parent() override
    {
        return m_parent;
    }

    virtual gate::PinState value() const override
    {
        return m_value;
    }

    // compute the name every time to save memory
    // revisit to determine if name should be stored
    // in a member
    std::string name() const
    {
        auto n = (m_parent ? m_parent->name() : "None")
            + ".fwd" + (direction() == PinDirection::INPUT ? "in" : "out")
            + std::to_string(m_index);
        return n;
    }

    virtual void value(gate::PinState newVal) override
    {
        m_value = newVal;
        for (auto peer : m_peers)
        {
            peer->value(newVal);
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

    virtual void connect(IPin* peer) override
    {
        m_peers.push_back(peer);
    }

    virtual std::vector<IPin*>& peers() override
    {
        return m_peers;
    }

protected:
    IComponent* m_parent;
    gate::PinState m_value;
    std::vector<IPin*> m_peers;
    int64_t m_index;
};

using ForwardInputPin  = ForwardPin<PinDirection::INPUT>;
using ForwardOutputPin = ForwardPin<PinDirection::OUTPUT>;

} // namespace component

#endif // FORWARD_PIN_HPP__ 
