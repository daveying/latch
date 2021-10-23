#ifndef IPIN_HPP_
#define IPIN_HPP_

#include <cstdint>
#include <vector>
#include <string>

namespace gate
{

using PinStateType = uint8_t;

enum class PinState : PinStateType
{
    Low  = 0,
    High = 1
};

inline PinState operator~(PinState value)
{
    if (value == PinState::Low)
    {
        return PinState::High;
    }
    else
    {
        return PinState::Low;
    }
    return PinState::Low;
}

inline std::string pinStateStr(const PinState& state)
{
    return state == PinState::High ? "High" : "Low";
}

class IGate;
class IPin
{
public:
    virtual const IGate* parent() const = 0;
    virtual PinState value() const      = 0;
    virtual void value(PinState newVal) = 0;
    virtual int64_t index() const       = 0;
    virtual void index(int64_t)         = 0;
};

class ISourcePin : public IPin
{
public:
    virtual void connect(IPin* peer)    = 0;
    virtual std::vector<IPin*>& peers() = 0;
};

} // namespace gate

#endif
