#include <ORGate.hpp>

#include <gtest/gtest.h>

TEST(ORGate, DefaultState)
{
    gate::ORGate orGate;
    EXPECT_EQ(orGate.input(0)->value(), gate::PinState::Low);
    EXPECT_EQ(orGate.input(1)->value(), gate::PinState::Low);
    EXPECT_EQ(orGate.output(0)->value(), gate::PinState::Low);
}

TEST(ORGate, TruthTable)
{
    gate::ORGate orGate;

    auto in0 = orGate.input(0);
    auto in1 = orGate.input(1);
    auto out = orGate.output(0);

    // in0 = Low, in1 = Low
    EXPECT_EQ(out->value(), gate::PinState::Low);

    // in0 = High, in1 = Low
    in0->value(gate::PinState::High);
    EXPECT_EQ(out->value(), gate::PinState::High);
    // in0 = High, in1 = High
    in1->value(gate::PinState::High);
    EXPECT_EQ(out->value(), gate::PinState::High);
    // in0 = Low, in1 = High
    in0->value(gate::PinState::Low);
    EXPECT_EQ(out->value(), gate::PinState::High);
}
