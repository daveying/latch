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

#include <ALUTests.hpp>

#include <gtest/gtest.h>

namespace component
{

TEST_F(BinaryAdderTests, BinaryAdder1)
{
    auto binaryAdder1 = BinaryAdder<1>::create("binaryAdder1");
    binaryAdder1->initialize();

    auto A0   = binaryAdder1->pin(0);
    auto B0   = binaryAdder1->pin(1);
    auto Ci   = binaryAdder1->pin(2);
    auto Sum0 = binaryAdder1->pin(3);
    auto Co   = binaryAdder1->pin(4);

    sched::waitTillSteady();
    // A0: 0, B0: 0, Ci: 0 -> Sum0: 0, Co: 0
    EXPECT_EQ(Sum0->value(), PinState::Low);
    EXPECT_EQ(Co->value(), PinState::Low);
    // A0: 1, B0: 0, Ci: 0 -> Sum0: 1, Co: 0
    A0->value(PinState::High);
    B0->value(PinState::Low);
    Ci->value(PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::High);
    EXPECT_EQ(Co->value(), PinState::Low);
    // A0: 0, B0: 1, Ci: 0 -> Sum0: 1, Co: 0
    A0->value(PinState::High);
    B0->value(PinState::Low);
    Ci->value(PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::High);
    EXPECT_EQ(Co->value(), PinState::Low);
    // A0: 1, B0: 1, Ci: 0 -> Sum0: 0, Co: 1
    A0->value(PinState::High);
    B0->value(PinState::High);
    Ci->value(PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::Low);
    EXPECT_EQ(Co->value(), PinState::High);
    // A0: 0, B0: 0, Ci: 1 -> Sum0: 1, Co: 0
    A0->value(PinState::Low);
    B0->value(PinState::Low);
    Ci->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::High);
    EXPECT_EQ(Co->value(), PinState::Low);
    // A0: 1, B0: 0, Ci: 1 -> Sum0: 0, Co: 1
    A0->value(PinState::High);
    B0->value(PinState::Low);
    Ci->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::Low);
    EXPECT_EQ(Co->value(), PinState::High);
    // A0: 0, B0: 1, Ci: 1 -> Sum0: 0, Co: 1
    A0->value(PinState::Low);
    B0->value(PinState::High);
    Ci->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::Low);
    EXPECT_EQ(Co->value(), PinState::High);
    // A0: 1, B0: 1, Ci: 1 -> Sum0: 1, Co: 1
    A0->value(PinState::High);
    B0->value(PinState::High);
    Ci->value(PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Sum0->value(), PinState::High);
    EXPECT_EQ(Co->value(), PinState::High);
}

TEST_F(BinaryAdderTests, BinaryAdder1AddTest)
{
    binaryAdderAddTest(std::make_index_sequence<1>{});
}

TEST_F(BinaryAdderTests, BinaryAdder4)
{
    binaryAdderAddTest(std::make_index_sequence<4>{});
}

TEST_F(BinaryAdderTests, BinaryAdder6)
{
    binaryAdderAddTest(std::make_index_sequence<6>{});
}

TEST_F(BinaryAdderTests, BinaryAdder6Overflow)
{
    binaryAdderOverflowTest(std::make_index_sequence<6>{});
}

TEST_F(BinaryAdderTests, BinaryAdder8Overflow)
{
    binaryAdderOverflowTest(std::make_index_sequence<8>{});
}

TEST_F(BinaryAdderTests, BinaryAdder16Overflow)
{
    binaryAdderOverflowTest(std::make_index_sequence<16>{});
}

TEST_F(BinaryAdderTests, BinaryAdder32Overflow)
{
    binaryAdderOverflowTest(std::make_index_sequence<32>{});
}

TEST_F(BinaryAdderTests, BinaryAdder64Overflow)
{
    binaryAdderOverflowTest(std::make_index_sequence<64>{});
}
} // namespace component
