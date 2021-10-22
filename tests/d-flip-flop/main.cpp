#include <NORGate.hpp>
#include <ANDGate.hpp>
#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(DFlipFLop, DFlipFlopTruthTable)
{
    gate::ZeroDelayOutputPin D{nullptr};
    gate::ZeroDelayOutputPin E{nullptr};

    gate::NOTGateDelayed invertor;
    gate::ANDGate pulseGate;

    gate::NOTGate notGate;
    gate::ANDGate andGate0;
    gate::ANDGate andGate1;
    gate::NORGate norGate0;
    gate::NORGate norGate1;

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    D.connect(andGate0.input(0));
    D.connect(notGate.input(0));

    auto E2 = pulseGate.output(0);
    E2->connect(andGate0.input(1));
    E2->connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    sched::waitTillSteady();

    auto Q = norGate1.output(0);

    sched::waitTillSteady();
    // D Low, E Low
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High, E Low
    D.value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // E -> High
    E.value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::High);

    // D -> Low
    D.value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::High);

    // E -> Low
    E.value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::High);

    // E -> pulse
    E.value(gate::PinState::High);
    sched::waitTillSteady();
    E.value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);
}

TEST(DFlipFLop, Pulse)
{
    sched::waitTillSteady();
    auto beginTime = sched::getCurrentTimestamp();

    gate::ZeroDelayOutputPin D{nullptr};
    gate::ZeroDelayOutputPin E{nullptr};

    gate::NOTGateDelayed invertor;
    gate::ANDGate pulseGate;

    gate::NOTGate notGate;
    gate::ANDGate andGate0;
    gate::ANDGate andGate1;
    gate::NORGate norGate0;
    gate::NORGate norGate1;

    E.connect(pulseGate.input(0));
    E.connect(invertor.input(0));
    invertor.output(0)->connect(pulseGate.input(1));

    D.connect(andGate0.input(0));
    D.connect(notGate.input(0));

    auto E2 = pulseGate.output(0);
    E2->connect(andGate0.input(1));
    E2->connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    sched::waitTillSteady();
    // here converge at timestamp 0 because invertor has no
    // value change when E connects to it.
    EXPECT_EQ(beginTime + 0, sched::getCurrentTimestamp());

    auto Q = norGate1.output(0);

    // D Low, E Low
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High, E Low
    D.value(gate::PinState::High);

    // E -> pulse
    sched::addEvent(1, [&]() { E.value(gate::PinState::High); });
    sched::addEvent(2, [&]() { E.value(gate::PinState::Low); });
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::High);
    EXPECT_EQ(E.value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 3, sched::getCurrentTimestamp());

    // D -> High, E Low
    D.value(gate::PinState::Low);

    // E -> pulse
    sched::addEvent(1, [&]() { E.value(gate::PinState::High); });
    sched::addEvent(2, [&]() { E.value(gate::PinState::Low); });
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(E.value(), gate::PinState::Low);
    EXPECT_EQ(beginTime + 6, sched::getCurrentTimestamp());
}
