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
    void setVal(uint64_t val, std::vector<IPin*>& pins, uint64_t bits)
    {
        for (uint64_t i = 0; i < bits; ++i)
        {
            uint64_t mask = 1ull << i;
            if ((mask & val) > 0)
            {
                pins[i]->value(PinState::High);
            }
            else
            {
                pins[i]->value(PinState::Low);
            }
        }
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
    template <size_t... I>
    void programCounterTest(std::index_sequence<I...>)
    {
        constexpr size_t BITS = sizeof...(I);
        auto pc = ComponentFactory::create("ProgramCounter" + std::to_string(BITS), "pc" + std::to_string(BITS));
        pc->initialize();
        auto CLK = pc->pin(0);
        auto J   = pc->pin(1);
        auto CE  = pc->pin(2);
        auto CO  = pc->pin(3);
        auto CLR = pc->pin(4);
        std::array<IPin*, BITS> Da {pc->pin(5 + I)...};
        std::array<IPin*, BITS> Qa {pc->pin(5 + BITS + I)...};
        auto RC  = pc->pin(5 + 2 * BITS);
        std::vector<IPin*> D(Da.begin(), Da.end());
        std::vector<IPin*> Q(Qa.begin(), Qa.end());

        ASSERT_EQ(CLK->value(), PinState::Low);
        ASSERT_EQ(J->value(), PinState::Low);
        ASSERT_EQ(CE->value(), PinState::Low);
        ASSERT_EQ(CO->value(), PinState::Low);
        ASSERT_EQ(CLR->value(), PinState::Low);
        ASSERT_EQ(RC->value(), PinState::Low);

        checkVal(0, D, BITS);
        checkVal(0, Q, BITS);

        CE->value(PinState::High); // counter enable
        CO->value(PinState::High); // counter out
        sched::waitTillSteady();

        for (uint64_t i = 0; i < BITS * 4; ++i)
        {
            CLK->value(PinState::High);
            sched::waitTillSteady();
            CLK->value(PinState::Low);
            sched::waitTillSteady();
            checkVal(i + 1, Q, BITS);
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

TEST_F(ProgramCounterTests, MSJKFlipFlopWithPresetClearTruthTable)
{
    auto msJK = ComponentFactory::create("MSJKFlipFlopWithPresetClear", "msJK");
    msJK->initialize();

    auto CLK   = msJK->pin(0);
    auto J     = msJK->pin(1);
    auto K     = msJK->pin(2);
    auto PR    = msJK->pin(3);
    auto CLR   = msJK->pin(4);
    auto Q     = msJK->pin(5);
    auto Qc    = msJK->pin(6);

    ASSERT_EQ(J->value(), PinState::Low);
    ASSERT_EQ(K->value(), PinState::Low);
    ASSERT_EQ(PR->value(), PinState::Low);
    ASSERT_EQ(CLR->value(), PinState::Low);
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // PR High, CLR Low
    PR->value(PinState::High);
    CLR->value(PinState::Low);

    CLK->value(PinState::Low);
    J->value(PinState::Low);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::High);
    J->value(PinState::Low);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::Low);
    J->value(PinState::High);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::High);
    J->value(PinState::High);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::Low);
    J->value(PinState::Low);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::High);
    J->value(PinState::Low);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::Low);
    J->value(PinState::High);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    CLK->value(PinState::High);
    J->value(PinState::High);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // PR High, CLR Low -> High
    CLR->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // PR Low, CLR High
    PR->value(PinState::Low);
    CLR->value(PinState::High);

    CLK->value(PinState::Low);
    J->value(PinState::Low);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::High);
    J->value(PinState::Low);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::Low);
    J->value(PinState::High);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::High);
    J->value(PinState::High);
    K->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::Low);
    J->value(PinState::Low);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::High);
    J->value(PinState::Low);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::Low);
    J->value(PinState::High);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    CLK->value(PinState::High);
    J->value(PinState::High);
    K->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // PR Low -> High, CLR High
    PR->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // PR Low, CLR Low
    CLR->value(PinState::Low);
    sched::waitTillSteady();
    PR->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // J High, K High, CLK High -> Low => toggle
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J Low, K Low, CLK High -> Low no change
    J->value(PinState::Low);
    K->value(PinState::Low);
    CLK->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J High, K Low, CLK High -> Low => Q High
    J->value(PinState::High);
    K->value(PinState::Low);
    CLK->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // J Low, K High, CLK High -> Low => Q Low
    J->value(PinState::Low);
    K->value(PinState::High);
    CLK->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);

    // J High, K High, CLK High -> Low => toggle
    J->value(PinState::High);
    K->value(PinState::High);
    CLK->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);

    // J High, K High, CLK High -> Low => toggle
    J->value(PinState::High);
    K->value(PinState::High);
    CLK->value(PinState::High);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::High);
    ASSERT_EQ(Qc->value(), PinState::Low);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    ASSERT_EQ(Q->value(), PinState::Low);
    ASSERT_EQ(Qc->value(), PinState::High);
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

TEST_F(ProgramCounterTests, SynchronousBinaryCounter)
{
    auto sbc = ComponentFactory::create("SynchronousBinaryCounter", "abc");
    sbc->initialize();

    auto CLK    = sbc->pin(0);
    auto CLR    = sbc->pin(1);
    auto LOAD   = sbc->pin(2);
    auto ENABLE = sbc->pin(3);
    auto D0     = sbc->pin(4);
    auto D1     = sbc->pin(5);
    auto D2     = sbc->pin(6);
    auto D3     = sbc->pin(7);
    auto Q0     = sbc->pin(8);
    auto Q1     = sbc->pin(9);
    auto Q2     = sbc->pin(10);
    auto Q3     = sbc->pin(11);
    auto RC     = sbc->pin(12);
    std::vector<IPin*> Q = {Q0, Q1, Q2, Q3};

    ASSERT_EQ(PinState::Low, CLK->value());
    ASSERT_EQ(PinState::Low, CLR->value());
    ASSERT_EQ(PinState::Low, LOAD->value());
    ASSERT_EQ(PinState::Low, ENABLE->value());
    ASSERT_EQ(PinState::Low, D0->value());
    ASSERT_EQ(PinState::Low, D1->value());
    ASSERT_EQ(PinState::Low, D2->value());
    ASSERT_EQ(PinState::Low, D3->value());
    ASSERT_EQ(PinState::Low, Q0->value());
    ASSERT_EQ(PinState::Low, Q1->value());
    ASSERT_EQ(PinState::Low, Q2->value());
    ASSERT_EQ(PinState::Low, Q3->value());
    ASSERT_EQ(PinState::Low, RC->value());

    ENABLE->value(PinState::High);
    sched::waitTillSteady(); // this is required, otherwise CLK will active first and then ENABLE
    for (size_t i = 0; i < 15; ++i)
    {
        std::cout << "i: " << i << std::endl;
        CLK->value(PinState::High);
        sched::waitTillSteady();
        checkVal(i + 1, Q, 4);
        if (i == 14)
        {
            ASSERT_EQ(PinState::High, RC->value());
            ENABLE->value(PinState::Low); // enable takes effect at next clock cycle
            sched::waitTillSteady();
        }
        else
        {
            ASSERT_EQ(PinState::Low, RC->value());
        }
        CLK->value(PinState::Low);
        sched::waitTillSteady();
        if (i == 14)
        {
            ASSERT_EQ(PinState::High, RC->value());
        }
        else
        {
            ASSERT_EQ(PinState::Low, RC->value());
        }
    }
    sched::waitTillSteady(); // this is required, otherwise CLK will active first and then ENABLE
    checkVal(15, Q, 4);
    ASSERT_EQ(PinState::High, RC->value());
    CLK->value(PinState::High);
    sched::waitTillSteady();
    checkVal(15, Q, 4);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    CLR->value(PinState::High); // clear
    sched::waitTillSteady();
    checkVal(0, Q, 4);
    CLK->value(PinState::High); // clear still set
    sched::waitTillSteady();
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    checkVal(0, Q, 4);
    CLR->value(PinState::Low);  // disable clear
    sched::waitTillSteady();
    ENABLE->value(PinState::High); // enable
    sched::waitTillSteady();
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    checkVal(1, Q, 4);

    LOAD->value(PinState::High); // load is already inside the Low part of this clock cycle
    D0->value(PinState::Low);
    D1->value(PinState::Low);
    D2->value(PinState::High);
    D3->value(PinState::High);
    sched::waitTillSteady();
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    checkVal(12, Q, 4);         // load established when clk goes high
    LOAD->value(PinState::Low);
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    checkVal(12, Q, 4);
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(13, Q, 4);
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(14, Q, 4);
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    ASSERT_EQ(PinState::High, RC->value()); // RC high
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(15, Q, 4);
    ASSERT_EQ(PinState::High, RC->value()); // RC high
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    ASSERT_EQ(PinState::Low, RC->value()); // RC low
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(0, Q, 4);
    CLK->value(PinState::High); // CLK spike should be filtered
    CLK->value(PinState::Low);
    sched::waitTillSteady();
    checkVal(0, Q, 4);
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(1, Q, 4);
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    ENABLE->value(PinState::Low); // disable
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(2, Q, 4);
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(2, Q, 4);          // inhibit
    CLK->value(PinState::High); // pusle
    sched::waitTillSteady();
    CLK->value(PinState::Low);  // begin of next clock cycle
    sched::waitTillSteady();
    checkVal(2, Q, 4);          // inhibit
}

TEST_F(ProgramCounterTests, ProgramCounter4)
{
    programCounterTest(std::make_index_sequence<4>{});
}
// TEST_F(ProgramCounterTests, ProgramCounter8)
// {
//     programCounterTest(std::make_index_sequence<8>{});
// }
// TEST_F(ProgramCounterTests, ProgramCounter16)
// {
//     programCounterTest(std::make_index_sequence<16>{});
// }
// TEST_F(ProgramCounterTests, ProgramCounter32)
// {
//     programCounterTest(std::make_index_sequence<32>{});
// }
// TEST_F(ProgramCounterTests, ProgramCounter64)
// {
//     programCounterTest(std::make_index_sequence<64>{});
// }
} // namespace component

