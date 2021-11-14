#ifndef TIMER_HPP__
#define TIMER_HPP__

#include <mutex>
#include <IComponent.hpp>

namespace component
{

class Timer : public IEventComponent
{
public:
    Timer(sched::Period period, const std::string& name = "");
    virtual void initialize() override;
    virtual const std::string& name() const override;
    virtual gate::IPin* pin(size_t idx) override;
    virtual void connect(gate::IPin* pin) override;
    virtual void disconnect() override;
    virtual void enable(gate::PinState value) override;
    void halt();
    virtual ~Timer() {}
protected:
    void timerEvent();

    sched::Period m_period;
    gate::IPin* m_pin;
    gate::PinState m_enabled;
    gate::PinState m_value;
    bool m_halt;
    // the public APIs will be called in different threads
    std::mutex m_eventMutex;
    std::string m_name;
};

} // namespace component

#endif // TIMER_HPP__
