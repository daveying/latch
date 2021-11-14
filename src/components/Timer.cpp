#include <Timer.hpp>

namespace component
{

Timer::Timer(sched::Period period)
    : m_period{period}
    , m_pin{nullptr}
    , m_value{gate::PinState::Low}
    , m_enabled{gate::PinState::Low}
    , m_halt{false}
{
}

void Timer::initialize()
{
    sched::addEvent(m_period, sched::Event::create(
                "Timer flip",
                [self = this] (sched::Timestamp) {
                    self->timerEvent();
                }
            ));
}

gate::IPin* Timer::pin(size_t idx)
{
    throw std::runtime_error("Timer::pin(): Invalid operation");
}

void Timer::halt()
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_halt = true;
}

void Timer::connect(gate::IPin* pin)
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_pin = pin;
}

void Timer::disconnect()
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_pin = nullptr;
}

void Timer::timerEvent()
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    if (m_halt)
    {
        return;
    }
    if (m_enabled == gate::PinState::High)
    {
        m_value = ~m_value;
    }
    if (m_pin != nullptr)
    {
        m_pin->value(m_value);
    }
    // NOTE: will not dead lock because scheduler will execute
    // newly added event after this event returns, even the
    // m_period is 0.
    sched::addEvent(m_period, sched::Event::create(
                "Timer flip",
                [self = this] (sched::Timestamp) {
                    self->timerEvent();
                }
            ));
}

void Timer::enable(gate::PinState value)
{
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_enabled = value;
}

} // namespace component
