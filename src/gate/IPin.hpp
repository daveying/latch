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

class IGate;
class IPin
{
public:
    virtual const IGate* parent() const = 0;
    virtual PinState value() const      = 0;
    virtual void value(PinState newVal) = 0;
};

class ISourcePin : public IPin
{
public:
    virtual void connect(IPin* peer)    = 0;
    virtual std::vector<IPin*>& peers() = 0;
};

} // namespace gate

#endif
