#include <NORGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(NORGate, DefaultState)
{
    gate::NORGate norGate;
    EXPECT_EQ(norGate.input(0)->value(), gate::PinState::Low);
    EXPECT_EQ(norGate.input(1)->value(), gate::PinState::Low);
    EXPECT_EQ(norGate.output(0)->value(), gate::PinState::High);
}

TEST(NORGate, TruthTable)
{
    gate::NORGate norGate;

    auto in0 = norGate.input(0);
    auto in1 = norGate.input(1);
    auto out = norGate.output(0);

    // in0 = Low, in1 = Low
    EXPECT_EQ(out->value(), gate::PinState::High);

    // in0 = High, in1 = Low
    in0->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
    // in0 = High, in1 = High
    in1->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
    // in0 = Low, in1 = High
    in0->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
}
