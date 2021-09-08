#include <ANDGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(ANDGate, DefaultState)
{
    gate::ANDGate andGate;
    EXPECT_EQ(andGate.input(0)->value(), gate::PinState::Low);
    EXPECT_EQ(andGate.input(1)->value(), gate::PinState::Low);
    EXPECT_EQ(andGate.output(0)->value(), gate::PinState::Low);
}

TEST(ANDGate, TruthTable)
{
    gate::ANDGate andGate;

    auto in0 = andGate.input(0);
    auto in1 = andGate.input(1);
    auto out = andGate.output(0);

    // in0 = Low, in1 = Low
    EXPECT_EQ(out->value(), gate::PinState::Low);

    // in0 = High, in1 = Low
    in0->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
    // in0 = High, in1 = High
    in1->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::High);
    // in0 = Low, in1 = High
    in0->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
}
