#include <NORGate.hpp>
#include <NANDGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(SRLatch, NORSRLatchTruthTable)
{
    gate::NORGate norGate0;
    gate::NORGate norGate1;
    EXPECT_EQ(norGate0.output(0)->value(), gate::PinState::High);
    EXPECT_EQ(norGate1.output(0)->value(), gate::PinState::High);
    norGate0.output(0)->connect(norGate1.input(0));
    sched::waitTillSteady();
    EXPECT_EQ(norGate1.output(0)->value(), gate::PinState::Low);

    norGate1.output(0)->connect(norGate0.input(1));
    sched::waitTillSteady();
    EXPECT_EQ(norGate0.output(0)->value(), gate::PinState::High);

    auto S  = norGate0.input(0);
    auto R  = norGate1.input(1);
    auto Qb = norGate0.output(0);
    auto Q  = norGate1.output(0);

    // S -> High
    S->value(gate::PinState::High);
    // R -> Low
    R->value(gate::PinState::Low);
    // S High, R Low
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::Low, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());

    // S -> Low
    S->value(gate::PinState::Low);
    // S Low, R Low
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::Low, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());

    // R -> High
    R->value(gate::PinState::High);
    // S Low, R High
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::Low, Q->value());

    // R -> Low
    R->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::Low, Q->value());

    // R -> High
    R->value(gate::PinState::High);
    // S -> High
    S->value(gate::PinState::High);
    // S High, R High
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::Low, Qb->value());
    EXPECT_EQ(gate::PinState::Low, Q->value());
}

TEST(SRLatch, NANDSRLatchTruthTable)
{
    gate::NANDGate nandGate0;
    gate::NANDGate nandGate1;
    EXPECT_EQ(nandGate0.output(0)->value(), gate::PinState::High);
    EXPECT_EQ(nandGate1.output(0)->value(), gate::PinState::High);
    nandGate0.output(0)->connect(nandGate1.input(0));
    sched::waitTillSteady();
    EXPECT_EQ(nandGate1.output(0)->value(), gate::PinState::High);
    nandGate1.output(0)->connect(nandGate0.input(1));
    sched::waitTillSteady();
    EXPECT_EQ(nandGate0.output(0)->value(), gate::PinState::High);

    auto S  = nandGate0.input(0);
    auto R  = nandGate1.input(1);
    auto Q  = nandGate0.output(0);
    auto Qb = nandGate1.output(0);

    // R -> High
    R->value(gate::PinState::High);
    // S Low, R High
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::Low, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());

    // S -> High
    S->value(gate::PinState::High);
    // S High, R High
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::Low, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());

    // R -> Low
    R->value(gate::PinState::Low);
    // S High, R Low
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::Low, Q->value());

    // R -> High
    R->value(gate::PinState::High);
    // S High, R High
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::Low, Q->value());

    // S -> Low
    S->value(gate::PinState::Low);
    // S Low, R High
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::Low, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());
    // R -> Low
    R->value(gate::PinState::Low);
    // S Low, R Low
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());

    // S -> High, R -> Low
    S->value(gate::PinState::High);
    R->value(gate::PinState::Low);
    // S High, R Low
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::Low, Q->value());
    // S -> Low
    S->value(gate::PinState::Low);
    // S Low, R Low
    sched::waitTillSteady();
    EXPECT_EQ(gate::PinState::High, Qb->value());
    EXPECT_EQ(gate::PinState::High, Q->value());
}
