/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2021 Xingpeng Da
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/////////////////////////////////////////////////////////////////////////////////

#include <NORGate.hpp>
#include <NANDGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(SRLatch, NORSRLatchTruthTable)
{
    gate::NORGate norGate0("norGate0");
    gate::NORGate norGate1("norGate1");
    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));
    norGate0.compute();
    // force one gate to compute faster
    // which one compute faster also matters
    sched::waitTillSteady();
    norGate1.compute();
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
    nandGate0.output(0)->connect(nandGate1.input(0));
    nandGate1.output(0)->connect(nandGate0.input(1));
    nandGate0.compute();
    // force one gate to compute faster
    // which one compute faster also matters
    sched::waitTillSteady();
    nandGate1.compute();
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
