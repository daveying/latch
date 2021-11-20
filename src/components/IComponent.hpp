#ifndef IEVENT_COMPONENT_HPP__
#define IEVENT_COMPONENT_HPP__

#include <IPin.hpp>
#include <IGate.hpp>
#include <IScheduler.hpp>

namespace component
{

class IEventComponent : public IComponent
{
public:
    virtual void connect(gate::IPin* pin)     = 0;
    virtual void disconnect()                 = 0;
    virtual void enable(gate::PinState value) = 0;
    virtual IComponent* subcomponent(int) override { return nullptr; };
    virtual ~IEventComponent() {}
};

} // namespace component

#endif // IEVENT_COMPONENT_HPP__
