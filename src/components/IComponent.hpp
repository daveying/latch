#ifndef IEVENT_COMPONENT_HPP__
#define IEVENT_COMPONENT_HPP__

#include <IPin.hpp>
#include <IScheduler.hpp>

namespace component
{

class IComponent
{
};

class IEventComponent : public IComponent
{
public:
    virtual void connect(gate::IPin* pin)     = 0;
    virtual void disconnect()                 = 0;
    virtual void enable(gate::PinState value) = 0;
    virtual ~IEventComponent() {}
};

} // namespace component

#endif // IEVENT_COMPONENT_HPP__
