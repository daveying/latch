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

#include <Register.hpp>

#include <gtest/gtest.h>

namespace component
{

class RegisterTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(RegisterTests, OneBit)
{
    std::unique_ptr<IComponent> oneBit = Register<1>::create("oneBit");
    ASSERT_NO_THROW(oneBit->initialize());

    auto Load  = oneBit->pin(0);
    auto Clock = oneBit->pin(1);
    auto D     = oneBit->pin(2);
    auto Q     = oneBit->pin(3);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q->value(), PinState::Low);

    // D -> High, Load Low
    D->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), PinState::Low);
    EXPECT_EQ(Clock->value(), PinState::Low);

    // D High, Load High
    Load->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), PinState::High);
}

TEST_F(RegisterTests, FourBit)
{
    std::unique_ptr<IComponent> fourBit = Register<4>::create("fourBit");
    ASSERT_NO_THROW(fourBit->initialize());

    auto Load  = fourBit->pin(0);
    auto Clock = fourBit->pin(1);
    auto D0    = fourBit->pin(2);
    auto D1    = fourBit->pin(3);
    auto D2    = fourBit->pin(4);
    auto D3    = fourBit->pin(5);
    auto Q0    = fourBit->pin(6);
    auto Q1    = fourBit->pin(7);
    auto Q2    = fourBit->pin(8);
    auto Q3    = fourBit->pin(9);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);

    // D -> High, Load Low
    D0->value(PinState::High);
    D1->value(PinState::Low);
    D2->value(PinState::High);
    D3->value(PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Clock->value(), PinState::Low);

    // D High, Load High
    Load->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::High);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::High);
    EXPECT_EQ(Q3->value(), PinState::Low);
}

TEST_F(RegisterTests, EightBit)
{
    std::unique_ptr<IComponent> eightBit = Register<8>::create("eightBit");
    ASSERT_NO_THROW(eightBit->initialize());

    auto Load  = eightBit->pin(0);
    auto Clock = eightBit->pin(1);
    auto D0    = eightBit->pin(2);
    auto D1    = eightBit->pin(3);
    auto D2    = eightBit->pin(4);
    auto D3    = eightBit->pin(5);
    auto D4    = eightBit->pin(6);
    auto D5    = eightBit->pin(7);
    auto D6    = eightBit->pin(8);
    auto D7    = eightBit->pin(9);
    auto Q0    = eightBit->pin(10);
    auto Q1    = eightBit->pin(11);
    auto Q2    = eightBit->pin(12);
    auto Q3    = eightBit->pin(13);
    auto Q4    = eightBit->pin(14);
    auto Q5    = eightBit->pin(15);
    auto Q6    = eightBit->pin(16);
    auto Q7    = eightBit->pin(17);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);

    // D -> High, Load Low
    D0->value(PinState::High);
    D1->value(PinState::Low);
    D2->value(PinState::High);
    D3->value(PinState::Low);
    D4->value(PinState::High);
    D5->value(PinState::High);
    D6->value(PinState::Low);
    D7->value(PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
    EXPECT_EQ(Clock->value(), PinState::Low);

    // D High, Load High
    Load->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::High);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::High);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::High);
    EXPECT_EQ(Q5->value(), PinState::High);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
}

TEST_F(RegisterTests, sixteenBit)
{
    std::unique_ptr<IComponent> sixteenBit = Register<16>::create("sixteenBit");
    ASSERT_NO_THROW(sixteenBit->initialize());

    auto Load  = sixteenBit->pin(0);
    auto Clock = sixteenBit->pin(1);
    auto D0    = sixteenBit->pin(2);
    auto D1    = sixteenBit->pin(3);
    auto D2    = sixteenBit->pin(4);
    auto D3    = sixteenBit->pin(5);
    auto D4    = sixteenBit->pin(6);
    auto D5    = sixteenBit->pin(7);
    auto D6    = sixteenBit->pin(8);
    auto D7    = sixteenBit->pin(9);
    auto D10   = sixteenBit->pin(10 + 0);
    auto D11   = sixteenBit->pin(10 + 1);
    auto D12   = sixteenBit->pin(10 + 2);
    auto D13   = sixteenBit->pin(10 + 3);
    auto D14   = sixteenBit->pin(10 + 4);
    auto D15   = sixteenBit->pin(10 + 5);
    auto D16   = sixteenBit->pin(10 + 6);
    auto D17   = sixteenBit->pin(10 + 7);
    auto Q0    = sixteenBit->pin(18 + 0);
    auto Q1    = sixteenBit->pin(18 + 1);
    auto Q2    = sixteenBit->pin(18 + 2);
    auto Q3    = sixteenBit->pin(18 + 3);
    auto Q4    = sixteenBit->pin(18 + 4);
    auto Q5    = sixteenBit->pin(18 + 5);
    auto Q6    = sixteenBit->pin(18 + 6);
    auto Q7    = sixteenBit->pin(18 + 7);
    auto Q10   = sixteenBit->pin(26 + 0);
    auto Q11   = sixteenBit->pin(26 + 1);
    auto Q12   = sixteenBit->pin(26 + 2);
    auto Q13   = sixteenBit->pin(26 + 3);
    auto Q14   = sixteenBit->pin(26 + 4);
    auto Q15   = sixteenBit->pin(26 + 5);
    auto Q16   = sixteenBit->pin(26 + 6);
    auto Q17   = sixteenBit->pin(26 + 7);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
    EXPECT_EQ(Q10->value(), PinState::Low);
    EXPECT_EQ(Q11->value(), PinState::Low);
    EXPECT_EQ(Q12->value(), PinState::Low);
    EXPECT_EQ(Q13->value(), PinState::Low);
    EXPECT_EQ(Q14->value(), PinState::Low);
    EXPECT_EQ(Q15->value(), PinState::Low);
    EXPECT_EQ(Q16->value(), PinState::Low);
    EXPECT_EQ(Q17->value(), PinState::Low);

    // D -> High, Load Low
    D0->value(PinState::High);
    D1->value(PinState::Low);
    D2->value(PinState::High);
    D3->value(PinState::Low);
    D4->value(PinState::High);
    D5->value(PinState::High);
    D6->value(PinState::Low);
    D7->value(PinState::Low);
    D10->value(PinState::High);
    D11->value(PinState::Low);
    D12->value(PinState::High);
    D13->value(PinState::Low);
    D14->value(PinState::High);
    D15->value(PinState::High);
    D16->value(PinState::Low);
    D17->value(PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
    EXPECT_EQ(Q10->value(), PinState::Low);
    EXPECT_EQ(Q11->value(), PinState::Low);
    EXPECT_EQ(Q12->value(), PinState::Low);
    EXPECT_EQ(Q13->value(), PinState::Low);
    EXPECT_EQ(Q14->value(), PinState::Low);
    EXPECT_EQ(Q15->value(), PinState::Low);
    EXPECT_EQ(Q16->value(), PinState::Low);
    EXPECT_EQ(Q17->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
    EXPECT_EQ(Q10->value(), PinState::Low);
    EXPECT_EQ(Q11->value(), PinState::Low);
    EXPECT_EQ(Q12->value(), PinState::Low);
    EXPECT_EQ(Q13->value(), PinState::Low);
    EXPECT_EQ(Q14->value(), PinState::Low);
    EXPECT_EQ(Q15->value(), PinState::Low);
    EXPECT_EQ(Q16->value(), PinState::Low);
    EXPECT_EQ(Q17->value(), PinState::Low);
    EXPECT_EQ(Clock->value(), PinState::Low);

    // D High, Load High
    Load->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::Low);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::Low);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::Low);
    EXPECT_EQ(Q5->value(), PinState::Low);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
    EXPECT_EQ(Q10->value(), PinState::Low);
    EXPECT_EQ(Q11->value(), PinState::Low);
    EXPECT_EQ(Q12->value(), PinState::Low);
    EXPECT_EQ(Q13->value(), PinState::Low);
    EXPECT_EQ(Q14->value(), PinState::Low);
    EXPECT_EQ(Q15->value(), PinState::Low);
    EXPECT_EQ(Q16->value(), PinState::Low);
    EXPECT_EQ(Q17->value(), PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), PinState::High);
    EXPECT_EQ(Q1->value(), PinState::Low);
    EXPECT_EQ(Q2->value(), PinState::High);
    EXPECT_EQ(Q3->value(), PinState::Low);
    EXPECT_EQ(Q4->value(), PinState::High);
    EXPECT_EQ(Q5->value(), PinState::High);
    EXPECT_EQ(Q6->value(), PinState::Low);
    EXPECT_EQ(Q7->value(), PinState::Low);
    EXPECT_EQ(Q10->value(), PinState::High);
    EXPECT_EQ(Q11->value(), PinState::Low);
    EXPECT_EQ(Q12->value(), PinState::High);
    EXPECT_EQ(Q13->value(), PinState::Low);
    EXPECT_EQ(Q14->value(), PinState::High);
    EXPECT_EQ(Q15->value(), PinState::High);
    EXPECT_EQ(Q16->value(), PinState::Low);
    EXPECT_EQ(Q17->value(), PinState::Low);
}

TEST_F(RegisterTests, thirtyTwoBitInitialize)
{
    std::unique_ptr<IComponent> thirtyTwoBit = Register<32>::create("thirtyTwoBit");
    ASSERT_NO_THROW(thirtyTwoBit->initialize());
}

#ifdef DEEP_TEMPLATE_RECURSIVE_BUILD
TEST_F(RegisterTests, sixtyFourBitInitialize)
{
    std::unique_ptr<IComponent> sixtyFourBit = Register<64>::create("sixtyFourBit");
    ASSERT_NO_THROW(sixtyFourBit->initialize());
}
#endif
} // namespace component
