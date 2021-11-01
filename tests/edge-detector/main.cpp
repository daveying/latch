#include <NORGate.hpp>
#include <ANDGate.hpp>
#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(EdgeDetectorUnitDelay, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    gate::ZeroDelayOutputPin D{nullptr};
    gate::ZeroDelayOutputPin E{nullptr};

    gate::NOTGateDelayed invertor{"invertor"};
    gate::ANDGate pulseGate{"pulseGate"};

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    sched::waitTillSteady();
    // here converge at timestamp 0 because invertor has no
    // value change when E connects to it.
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    auto Q = pulseGate.output(0);

    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // E -> High
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(gate::PinState::High); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 2, sched::getCurrentTimestamp());

    // E -> Low
    sched::addEvent(1, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 4, sched::getCurrentTimestamp());
}

TEST(EdgeDetectorNonDelayed, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    gate::ZeroDelayOutputPin D{nullptr};
    gate::ZeroDelayOutputPin E{nullptr};

    gate::NOTGate invertor{"invertor"};
    gate::ANDGate pulseGate{"pulseGate"};

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    sched::waitTillSteady();
    // here converge at timestamp 0 because invertor has no
    // value change when E connects to it.
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    auto Q = pulseGate.output(0);

    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // E -> High
    sched::addEvent(1, sched::Event::create("E goes high", [&](sched::Timestamp) { E.value(gate::PinState::High); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 1, sched::getCurrentTimestamp());

    // E -> Low
    sched::addEvent(1, sched::Event::create("E goes low", [&](sched::Timestamp) { E.value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 2, sched::getCurrentTimestamp());
}
