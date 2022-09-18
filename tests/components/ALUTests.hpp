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
#ifndef ALU_TESTS_HPP__
#define ALU_TESTS_HPP__

#include <ALU.hpp>
#include <ComponentFactory.hpp>

#include <gtest/gtest.h>

namespace component
{

class BinaryAdderTests : public ::testing::Test
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
                ASSERT_EQ(pins[i]->value(), PinState::High);
            }
            else
            {
                ASSERT_EQ(pins[i]->value(), PinState::Low);
            }
        }
    }

    template <size_t... I>
    void binaryAdderAddTest(const std::index_sequence<I...>&)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS < 64, "Too many bits for 64bits system, will overflow");
        auto binaryAdder = BITS == 6
            ? BinaryAdder<6>::create("binaryAdder6")
            : ComponentFactory::create("BinaryAdder" + std::to_string(BITS), "binaryAdder" + std::to_string(BITS));
        binaryAdder->initialize();

        std::array<IPin*, BITS> A = {
            binaryAdder->pin(0 + I)...
        };
        std::array<IPin*, BITS> B = {
            binaryAdder->pin(BITS + I)...
        };
        auto Ci = binaryAdder->pin(2 * BITS);
        std::array<IPin*, BITS> Sum = {
            binaryAdder->pin(2 * BITS + 1 + I)...
        };
        auto Co = binaryAdder->pin(3 * BITS + 1);

        sched::waitTillSteady();

        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        ASSERT_EQ(Ci->value(), PinState::Low);
        ASSERT_EQ(Co->value(), PinState::Low);

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
                    ASSERT_EQ(Co->value(), PinState::Low);
                }
                else
                {
                    ASSERT_EQ(Co->value(), PinState::High);
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
                    ASSERT_EQ(Co->value(), PinState::Low);
                }
                else
                {
                    ++CoHighCount;
                    ASSERT_EQ(Co->value(), PinState::High);
                }
            }
        }
        // 1 + ... + 2^BITS
        ASSERT_EQ(((1 + overflowVal) * overflowVal) >> 1, CoHighCount);
    }

    template <size_t... I>
    void binaryAdderOverflowTest(const std::index_sequence<I...>&)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        auto binaryAdder = BITS == 6
            ? BinaryAdder<6>::create("binaryAdder6")
            : ComponentFactory::create("BinaryAdder" + std::to_string(BITS), "binaryAdder" + std::to_string(BITS));
        binaryAdder->initialize();

        std::array<IPin*, BITS> A = {
            binaryAdder->pin(0 + I)...
        };
        std::array<IPin*, BITS> B = {
            binaryAdder->pin(BITS + I)...
        };
        auto Ci = binaryAdder->pin(2 * BITS);
        std::array<IPin*, BITS> Sum = {
            binaryAdder->pin(2 * BITS + 1 + I)...
        };
        auto Co = binaryAdder->pin(3 * BITS + 1);

        sched::waitTillSteady();
        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        ASSERT_EQ(Ci->value(), PinState::Low);
        ASSERT_EQ(Co->value(), PinState::Low);

        uint64_t biggestVal = (BITS == 64 ? std::numeric_limits<uint64_t>::max() : ((1ull << BITS) - 1));
        setVal(1, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(0, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);

        setVal(biggestVal, A);
        setVal(1, B);
        sched::waitTillSteady();
        checkVal(0, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);

        setVal(biggestVal, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(biggestVal - 1, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);

        Ci->value(PinState::High);
        setVal(biggestVal, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(biggestVal, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);
    }
};

class ALUTests : public BinaryAdderTests
{
protected:
    template <size_t... I>
    void ALUAddTest(std::index_sequence<I...>)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        auto alu = BITS == 6
            ? ALU<6>::create("alu6")
            : ComponentFactory::create("ALU" + std::to_string(BITS), "alu" + std::to_string(BITS));
        alu->initialize();

        std::array<IPin*, BITS> A   = {alu->pin(I)...};
        std::array<IPin*, BITS> B   = {alu->pin(BITS + I)...};
        IPin* EO                    = alu->pin(2 * BITS);
        IPin* SUB                   = alu->pin(2 * BITS + 1);
        std::array<IPin*, BITS> Sum = {alu->pin(2 * BITS + 2 + I)...};
        IPin* Co                    = alu->pin(3 * BITS + 2);

        sched::waitTillSteady();

        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        ASSERT_EQ(EO->value(), PinState::Low);
        ASSERT_EQ(SUB->value(), PinState::Low);
        ASSERT_EQ(Co->value(), PinState::Low);

        uint64_t overflowVal = (1ull << BITS);
        EO->value(PinState::High);
        SUB->value(PinState::Low);
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
                    ASSERT_EQ(Co->value(), PinState::Low);
                }
                else
                {
                    ASSERT_EQ(Co->value(), PinState::High);
                }
            }
        }
    }
    template <size_t BITS>
    void setValSigned(int64_t val, std::array<IPin*, BITS>& pins) // set two's complement representation of val
    {
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        // -7 in 4 bits: 0b1001, -7 in 8 bits: 0b1111 1001, -7 in 16 bits: 0b1111 1111 1111 1001
        // so no matter the width, if the value is within the boundary, the lower BITS are the same
        uint64_t bits = *reinterpret_cast<uint64_t*>(&val);
        for (size_t i = 0; i < BITS; ++i)
        {
            if ((bits & (1ull << i)) > 0)
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
    void checkValSigned(int64_t val, std::array<IPin*, BITS>& pins)
    {
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        // -7 in 4 bits: 0b1001, -7 in 8 bits: 0b1111 1001, -7 in 16 bits: 0b1111 1111 1111 1001
        // so no matter the width, if the value is within the boundary, the lower BITS are the same
        uint64_t bits = *reinterpret_cast<uint64_t*>(&val);
        for (size_t i = 0; i < BITS; ++i)
        {
            if ((bits & (1ull << i)) > 0)
            {
                ASSERT_EQ(pins[i]->value(), PinState::High);
            }
            else
            {
                ASSERT_EQ(pins[i]->value(), PinState::Low);
            }
        }
    }

    template <size_t... I>
    void ALUSubstractionTest(std::index_sequence<I...>)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS < 64, "Too many bits for 64bits system, will overflow");
        auto alu = BITS == 6
            ? ALU<6>::create("alu6")
            : ComponentFactory::create("ALU" + std::to_string(BITS), "alu" + std::to_string(BITS));
        alu->initialize();

        std::array<IPin*, BITS> A   = {alu->pin(I)...};
        std::array<IPin*, BITS> B   = {alu->pin(BITS + I)...};
        IPin* EO                    = alu->pin(2 * BITS);
        IPin* SUB                   = alu->pin(2 * BITS + 1);
        std::array<IPin*, BITS> Sum = {alu->pin(2 * BITS + 2 + I)...};
        IPin* Co                    = alu->pin(3 * BITS + 2);

        sched::waitTillSteady();

        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        ASSERT_EQ(EO->value(), PinState::Low);
        ASSERT_EQ(SUB->value(), PinState::Low);
        ASSERT_EQ(Co->value(), PinState::Low);

        EO->value(PinState::High);
        SUB->value(PinState::High);
        int64_t overflowL = -(1ll << (BITS - 1));    // BITS = 8 => overflowL = -128
        int64_t overflowH = (1ll << (BITS - 1)) - 1; // BITS = 8 => overflowH = 127
        for (int64_t a = overflowL; a <= overflowH; ++a)
        {
            for (int64_t b = overflowL; b <= overflowH; ++b)
            {
                setValSigned(a, A); // convert 64 signed value to BITS signed value
                setValSigned(b, B);
                sched::waitTillSteady();
                int64_t result64bits = a - b;
                std::cout << "a = " << a << ", b = " << b << std::endl;
                std::cout << "overflowL = " << overflowL << ", overflowH = " << overflowH << ", result64bits = " << result64bits << std::endl;
                checkValSigned(result64bits, Sum);
            }
        }
    }
    template <size_t... I>
    void ALUOverflowTest(const std::index_sequence<I...>&)
    {
        constexpr size_t BITS = sizeof...(I);
        static_assert(BITS <= 64, "Too many bits for 64bits system, will overflow");
        auto alu = BITS == 6
            ? ALU<6>::create("binaryAdder6")
            : ComponentFactory::create("ALU" + std::to_string(BITS), "alu" + std::to_string(BITS));
        alu->initialize();

        std::array<IPin*, BITS> A   = {alu->pin(I)...};
        std::array<IPin*, BITS> B   = {alu->pin(BITS + I)...};
        IPin* EO                    = alu->pin(2 * BITS);
        IPin* SUB                   = alu->pin(2 * BITS + 1);
        std::array<IPin*, BITS> Sum = {alu->pin(2 * BITS + 2 + I)...};
        IPin* Co                    = alu->pin(3 * BITS + 2);

        sched::waitTillSteady();
        checkVal(0, A);
        checkVal(0, B);
        checkVal(0, Sum);
        ASSERT_EQ(EO->value(), PinState::Low);
        ASSERT_EQ(SUB->value(), PinState::Low);
        ASSERT_EQ(Co->value(), PinState::Low);

        EO->value(PinState::High);

        uint64_t biggestVal = (BITS == 64 ? std::numeric_limits<uint64_t>::max() : ((1ull << BITS) - 1));
        setVal(1, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(0, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);

        setVal(biggestVal, A);
        setVal(1, B);
        sched::waitTillSteady();
        checkVal(0, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);

        setVal(biggestVal, A);
        setVal(biggestVal, B);
        sched::waitTillSteady();
        checkVal(biggestVal - 1, Sum); // overflow
        ASSERT_EQ(Co->value(), PinState::High);
    }

};

} // namespace component

#endif // ALU_TESTS_HPP__
