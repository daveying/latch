#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(NOTGate, DefaultState)
{
    gate::NOTGate notGate;
    EXPECT_EQ(notGate.input(0)->value(), gate::PinState::Low);
    EXPECT_EQ(notGate.output(0)->value(), gate::PinState::High);
}

TEST(NOTGate, TruthTable)
{
    gate::NOTGate notGate;

    auto in  = notGate.input(0);
    auto out = notGate.output(0);

    // in = Low
    EXPECT_EQ(out->value(), gate::PinState::High);

    // in = High
    in->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), gate::PinState::Low);
}
