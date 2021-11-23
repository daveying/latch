#ifndef IPIN_HPP_
#define IPIN_HPP_

#include <cstdint>
#include <vector>
#include <string>

namespace component
{
class IComponent;
}

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
    PinState ret;
    if (value == PinState::Low)
    {
        ret = PinState::High;
    }
    else
    {
        ret = PinState::Low;
    }
    return ret;
}

inline std::string pinStateStr(const PinState& state)
{
    std::string ret;
    if (state == PinState::Low)
    {
        ret = "Low";
    }
    else
    {
        ret = "High";
    }
    return ret;
}

class IPin
{
public:
    virtual const component::IComponent* parent() const = 0;
    virtual component::IComponent* parent()             = 0;
    virtual PinState value() const                      = 0;
    virtual void value(PinState newVal)                 = 0;
    virtual int64_t index() const                       = 0;
    virtual void index(int64_t)                         = 0;
};

class ISourcePin : public IPin
{
public:
    virtual void connect(IPin* peer)    = 0;
    virtual std::vector<IPin*>& peers() = 0;
};

} // namespace gate

#endif
