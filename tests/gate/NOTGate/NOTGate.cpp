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

#include <NOTGate.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

TEST(NOTGate, DefaultState)
{
    component::NOTGate notGate;
    notGate.compute();
    EXPECT_EQ(notGate.input(0)->value(), component::PinState::Low);
    EXPECT_EQ(notGate.output(0)->value(), component::PinState::High);
}

TEST(NOTGate, TruthTable)
{
    component::NOTGate notGate;
    notGate.compute();

    auto in  = notGate.input(0);
    auto out = notGate.output(0);

    // in = Low
    EXPECT_EQ(out->value(), component::PinState::High);

    // in = High
    in->value(component::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(out->value(), component::PinState::Low);
}
