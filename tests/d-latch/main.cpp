#include <NORGate.hpp>
#include <ANDGate.hpp>
#include <NOTGate.hpp>

#include <gtest/gtest.h>

TEST(DLatch, DLatchTruthTable)
{
    gate::ZeroDelayOutputPin D{nullptr};
    gate::ZeroDelayOutputPin E{nullptr};

    gate::NOTGate notGate;
    gate::ANDGate andGate0;
    gate::ANDGate andGate1;
    gate::NORGate norGate0;
    gate::NORGate norGate1;

    D.connect(andGate0.input(0));
    D.connect(notGate.input(0));
    E.connect(andGate0.input(1));
    E.connect(andGate1.input(0));

    notGate.output(0)->connect(andGate1.input(1));

    andGate0.output(0)->connect(norGate0.input(0));
    andGate1.output(0)->connect(norGate1.input(1));

    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    auto Q = norGate1.output(0);

    // D Low, E Low
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High, E Low
    D.value(gate::PinState::High);
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // E -> High
    E.value(gate::PinState::High);
    EXPECT_EQ(Q->value(), gate::PinState::High);

    // D -> Low
    D.value(gate::PinState::Low);
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // E -> Low
    E.value(gate::PinState::Low);
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High
    D.value(gate::PinState::High);
    EXPECT_EQ(Q->value(), gate::PinState::Low);
}
