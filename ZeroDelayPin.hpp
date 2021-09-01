#ifndef ZERO_DELAY_PIN_HPP_
#define ZERO_DELAY_PIN_HPP_

#include <vector>

#include <IPin.hpp>

namespace gate
{

class ZeroDelayPin : public IPin
{
public:
    explicit ZeroDelayPin(IGate* parent)
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
        m_value = newVal;
    }
    virtual void connect(IPin* pear) override
    {
        m_peers.push_back(pear);
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
