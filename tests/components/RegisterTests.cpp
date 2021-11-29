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
    auto D     = oneBit->pin(1);
    auto Clock = oneBit->pin(2);
    auto Q     = oneBit->pin(3);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High, Load Low
    D->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(Clock->value(), gate::PinState::Low);

    // D High, Load High
    Load->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::High);
}

TEST_F(RegisterTests, FourBit)
{
    std::unique_ptr<IComponent> fourBit = Register<4>::create("fourBit");
    ASSERT_NO_THROW(fourBit->initialize());

    auto Load  = fourBit->pin(0);
    auto D0    = fourBit->pin(1);
    auto D1    = fourBit->pin(2);
    auto D2    = fourBit->pin(3);
    auto D3    = fourBit->pin(4);
    auto Clock = fourBit->pin(5);
    auto Q0    = fourBit->pin(6);
    auto Q1    = fourBit->pin(7);
    auto Q2    = fourBit->pin(8);
    auto Q3    = fourBit->pin(9);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);

    // D -> High, Load Low
    D0->value(gate::PinState::High);
    D1->value(gate::PinState::Low);
    D2->value(gate::PinState::High);
    D3->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);
    EXPECT_EQ(Clock->value(), gate::PinState::Low);

    // D High, Load High
    Load->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), gate::PinState::High);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::High);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);
}
} // namespace component
