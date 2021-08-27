#ifndef IPIN_HPP_
#define IPIN_HPP_

namespace gate
{

using PinStateType = uint8_t;

enum class PinState : PinStateType
{
    Low  = 0,
    High = 1
};

// following operators are defined under the assumption that there
// are only two pin states
static inline bool operator==(const PinState& lhs, const PinState& rhs)
{
    return static_cast<PinStateType>(lhs) == static_cast<PinStateType>(rhs);
}

static inline bool operator!=(const PinState& lhs, const PinState& rhs)
{
    return static_cast<PinStateType>(lhs) != static_cast<PinStateType>(rhs);
}

static inline PinState operator!(const PinState& val)
{
    return static_cast<PinState>(!static_cast<PinStateType>(val));
}

class IPin
{
public:
    virtual const IGate* parent() const = 0;
    virtual PinState value() const      = 0;
    virtual void value(PinState newVal) = 0;
};

} // namespace gate

#endif
