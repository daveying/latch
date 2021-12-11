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

#include <ComponentFactory.hpp>
#include <ProgramCounter.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

namespace component
{

class ProgramCounterTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ProgramCounterTests, MSJKFlipFlopTruthTable)
{
    auto msJK = ComponentFactory::create("MSJKFlipFlop", "msJK");
    msJK->initialize();

    auto Clock = msJK->pin(0);
    auto J     = msJK->pin(1);
    auto K     = msJK->pin(2);
    auto Q     = msJK->pin(3);
    auto Qc    = msJK->pin(4);

    ASSERT_EQ(J->value(), PinState::Low);
    ASSERT_EQ(K->value(), PinState::Low);
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J Low, K Low
    Clock->value(PinState::High);
    Clock->value(PinState::Low);   // Clock pulse

    sched::waitTillSteady();

    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J High, K Low, Clock pulse => Q High, Qc Low
    J->value(PinState::High);
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();      // NOTE: Clock cannot goes High and Low at the same timestamp
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // J Low, K High, Clock pulse => Q Low, Qc High
    J->value(PinState::Low);
    K->value(PinState::High);
    sched::waitTillSteady();
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J Low, K Low, Clock pulse => no change
    K->value(PinState::Low);
    sched::waitTillSteady();
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J High, K Low, Clock pulse => Q High, Qc Low
    J->value(PinState::High);
    K->value(PinState::Low);
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // J Low, K Low, Clock pulse => no change
    J->value(PinState::Low);
    sched::waitTillSteady();
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // J High, K High, Clock pulse => toggle
    J->value(PinState::High);
    K->value(PinState::High);
    sched::waitTillSteady();
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // pulse
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // pulse
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // pulse
    Clock->value(PinState::High); // Clock Low -> High
    sched::waitTillSteady();
    Clock->value(PinState::Low);  // Clock High -> Low
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);
}

} // namespace component

