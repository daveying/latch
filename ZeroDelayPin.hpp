#ifndef ZERO_DELAY_PIN_HPP_
#define ZERO_DELAY_PIN_HPP_

#include <IPin.h>

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
protected:
    IGate* m_parent;
    PinState m_value;
};

} // namespace gate

#endif
