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
    void checkVal(uint64_t val, std::vector<IPin*>& pins, uint64_t bits)
    {
        for (uint64_t i = 0; i < bits; ++i)
        {
            uint64_t mask = 1ull << i;
            if ((mask & val) > 0)
            {
                ASSERT_EQ(pins[i]->value(), PinState::High);
            }
            else
            {
                ASSERT_EQ(pins[i]->value(), PinState::Low);
            }
        }
    }
    template <size_t BITS>
    void binaryCounterTest()
    {
        auto bc = ComponentFactory::create("BinaryCounter" + std::to_string(BITS), "bc" + std::to_string(BITS));
        bc->initialize();
        auto Clock = bc->pin(0);
        std::vector<IPin*> C;
        for (size_t i = 0; i < BITS; ++i)
        {
            C.push_back(bc->pin(1 + i));
        }

        for (uint64_t i = 0; i < BITS * BITS; ++i)
        {
            Clock->value(PinState::High);
            sched::waitTillSteady();
            Clock->value(PinState::Low);
            sched::waitTillSteady();
            checkVal(i + 1, C, BITS);
        }
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

TEST_F(ProgramCounterTests, BinaryCounter1)
{
    binaryCounterTest<1>();
}
TEST_F(ProgramCounterTests, BinaryCounter2)
{
    binaryCounterTest<2>();
}
TEST_F(ProgramCounterTests, BinaryCounter4)
{
    binaryCounterTest<4>();
}
TEST_F(ProgramCounterTests, BinaryCounter8)
{
    binaryCounterTest<8>();
}
TEST_F(ProgramCounterTests, BinaryCounter16)
{
    binaryCounterTest<16>();
}
TEST_F(ProgramCounterTests, BinaryCounter32)
{
    binaryCounterTest<32>();
}
TEST_F(ProgramCounterTests, BinaryCounter64)
{
    binaryCounterTest<64>();
}

} // namespace component

