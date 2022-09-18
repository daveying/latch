/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2022 Xingpeng Da
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
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(NORGate, DefaultState)
{
    component::NORGate norGate;
    norGate.compute();
    EXPECT_EQ(norGate.input(0)->value(), component::PinState::Low);
    EXPECT_EQ(norGate.input(1)->value(), component::PinState::Low);
    EXPECT_EQ(norGate.output(0)->value(), component::PinState::High);
}

TEST(NORGate, TruthTable)
{
    component::NORGate norGate;
    norGate.compute();

    auto in0 = norGate.input(0);
    auto in1 = norGate.input(1);
    auto out = norGate.output(0);

    // in0 = Low, in1 = Low
    EXPECT_EQ(out->value(), component::PinState::High);

    // in0 = High, in1 = Low
    in0->value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::Low);
    // in0 = High, in1 = High
    in1->value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::Low);
    // in0 = Low, in1 = High
    in0->value(component::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::Low);
}
