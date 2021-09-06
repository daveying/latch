#include <NANDGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(NANDGate, DefaultState)
{
    gate::NANDGate nandGate;
    EXPECT_EQ(nandGate.input(0)->value(), gate::PinState::Low);
    EXPECT_EQ(nandGate.input(1)->value(), gate::PinState::Low);
    EXPECT_EQ(nandGate.output(0)->value(), gate::PinState::High);
}

TEST(NANDGate, TruthTable)
{
    gate::NANDGate nandGate;

    auto in0 = nandGate.input(0);
    auto in1 = nandGate.input(1);
    auto out = nandGate.output(0);

    // in0 = Low, in1 = Low
    EXPECT_EQ(out->value(), gate::PinState::High);

    // in0 = High, in1 = Low
    in0->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::High);
    // in0 = High, in1 = High
    in1->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
    // in0 = Low, in1 = High
    in0->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::High);
}
