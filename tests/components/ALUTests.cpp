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

#include <ALU.hpp>
#include <ComponentFactory.hpp>

#include <gtest/gtest.h>

namespace component
{

class ALUTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
    template <size_t BITS>
    void setVal(uint64_t val, std::array<IPin*, BITS>& pins)
    {
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        for (size_t i = 0; i < BITS; ++i)
        {
            if ((val & (1ull << i)) > 0)
            {
                pins[i]->value(PinState::High);
            }
            else
            {
                pins[i]->value(PinState::Low);
            }
        }
    }
    template <size_t BITS>
    void checkVal(uint64_t val, std::array<IPin*, BITS>& pins)
    {
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        for (size_t i = 0; i < BITS; ++i)
        {
            if ((val & (1ull << i)) > 0)
            {
                EXPECT_EQ(pins[i]->value(), PinState::High);
            }
            else
            {
                EXPECT_EQ(pins[i]->value(), PinState::Low);
            }
        }
    }

    template <size_t... I>
    void aluAddTest(std::index_sequence<I...>)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS < 64, "Too many bits for 64bits system, will overflow");
        auto alu = BITS == 6
            ? ALU<6>::create("alu7")
            : ComponentFactory::create("ALU" + std::to_string(BITS), "alu" + std::to_string(BITS));
        alu->initialize();

        std::array<IPin*, BITS> A = {
            alu->pin(0 + I)...
        };
        std::array<IPin*, BITS> B = {
            alu->pin(BITS + I)...
        };
        auto Ci = alu->pin(2 * BITS);
        std::array<IPin*, BITS> Sum = {
            alu->pin(2 * BITS + 1 + I)...
        };
        auto Co = alu->pin(3 * BITS + 1);

        sched::waitTillSteady();

        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        EXPECT_EQ(Ci->value(), PinState::Low);
        EXPECT_EQ(Co->value(), PinState::Low);

        uint64_t overflowVal = (1ull << BITS);
        for (uint64_t a = 0; a < overflowVal; ++a)
        {
            for (uint64_t b = 0; b < overflowVal; ++b)
            {
                setVal(a, A);
                setVal(b, B);
                sched::waitTillSteady();
                uint64_t mask = (1ull << BITS) - 1;
                checkVal(mask & (a + b), Sum); // only check first BITS of a + b
                if (a + b < overflowVal)
                {
                    EXPECT_EQ(Co->value(), PinState::Low);
                }
                else
                {
                    EXPECT_EQ(Co->value(), PinState::High);
                }
            }
        }

        size_t CoHighCount = 0;
        Ci->value(PinState::High);
        for (uint64_t a = 0; a < overflowVal; ++a)
        {
            for (uint64_t b = 0; b < overflowVal; ++b)
            {
                setVal(a, A);
                setVal(b, B);
                sched::waitTillSteady();
                uint64_t mask = (1ull << BITS) - 1;
                checkVal(mask & (a + b + 1), Sum);
                if (a + b + 1 < overflowVal)
                {
                    EXPECT_EQ(Co->value(), PinState::Low);
                }
                else
                {
                    ++CoHighCount;
                    EXPECT_EQ(Co->value(), PinState::High);
                }
            }
        }
        // 1 + ... + 2^BITS
        EXPECT_EQ(((1 + overflowVal) * overflowVal) >> 1, CoHighCount);
    }

    template <size_t... I>
    void aluOverflowTest(std::index_sequence<I...>)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        auto alu = BITS == 6
            ? ALU<6>::create("alu7")
            : ComponentFactory::create("ALU" + std::to_string(BITS), "alu" + std::to_string(BITS));
        alu->initialize();

        std::array<IPin*, BITS> A = {
            alu->pin(0 + I)...
        };
        std::array<IPin*, BITS> B = {
            alu->pin(BITS + I)...
        };
        auto Ci = alu->pin(2 * BITS);
        std::array<IPin*, BITS> Sum = {
            alu->pin(2 * BITS + 1 + I)...
        };
        auto Co = alu->pin(3 * BITS + 1);

        sched::waitTillSteady();
        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        EXPECT_EQ(Ci->value(), PinState::Low);
        EXPECT_EQ(Co->value(), PinState::Low);

        uint64_t biggestVal = (BITS == 64 ? std::numeric_limits<uint64_t>::max() : ((1ull << BITS) - 1));
        setVal(1, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(0, Sum); // overflow
        EXPECT_EQ(Co->value(), PinState::High);

        setVal(biggestVal, A);
        setVal(1, B);
        sched::waitTillSteady();
        checkVal(0, Sum); // overflow
        EXPECT_EQ(Co->value(), PinState::High);

        setVal(biggestVal, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(biggestVal - 1, Sum); // overflow
        EXPECT_EQ(Co->value(), PinState::High);

        Ci->value(PinState::High);
        setVal(biggestVal, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(biggestVal, Sum); // overflow
        EXPECT_EQ(Co->value(), PinState::High);
    }
};

TEST_F(ALUTests, ALU1)
{
    auto alu1 = ALU<1>::create("alu1");
    alu1->initialize();

    auto A0   = alu1->pin(0);
    auto B0   = alu1->pin(1);
    auto Ci   = alu1->pin(2);
    auto Sum0 = alu1->pin(3);
    auto Co   = alu1->pin(4);

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

TEST_F(ALUTests, ALU1AddTest)
{
    aluAddTest(std::make_index_sequence<1>{});
}

TEST_F(ALUTests, ALU4)
{
    aluAddTest(std::make_index_sequence<4>{});
}

TEST_F(ALUTests, ALU6)
{
    aluAddTest(std::make_index_sequence<6>{});
}

TEST_F(ALUTests, ALU6Overflow)
{
    aluOverflowTest(std::make_index_sequence<6>{});
}

TEST_F(ALUTests, ALU8Overflow)
{
    aluOverflowTest(std::make_index_sequence<8>{});
}

TEST_F(ALUTests, ALU16Overflow)
{
    aluOverflowTest(std::make_index_sequence<16>{});
}

TEST_F(ALUTests, ALU32Overflow)
{
    aluOverflowTest(std::make_index_sequence<32>{});
}

TEST_F(ALUTests, ALU64Overflow)
{
    aluOverflowTest(std::make_index_sequence<64>{});
}
} // namespace component
