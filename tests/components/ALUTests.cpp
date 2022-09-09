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

TEST_F(ALUTests, ALU1)
{
    ALUAddTest(std::make_index_sequence<1>{});
    ALUSubstractionTest(std::make_index_sequence<1>{});
}

TEST_F(ALUTests, ALU4)
{
    ALUAddTest(std::make_index_sequence<4>{});
    ALUSubstractionTest(std::make_index_sequence<4>{});
}

TEST_F(ALUTests, ALU6)
{
    ALUAddTest(std::make_index_sequence<6>{});
    ALUSubstractionTest(std::make_index_sequence<6>{});
}

TEST_F(ALUTests, ALU8)
{
    // ALUAddTest(std::make_index_sequence<8>{});
    // ALUSubstractionTest(std::make_index_sequence<8>{});
    ALUOverflowTest(std::make_index_sequence<8>{});
}

TEST_F(ALUTests, ALU16)
{
    ALUOverflowTest(std::make_index_sequence<16>{});
}

TEST_F(ALUTests, ALU32)
{
    ALUOverflowTest(std::make_index_sequence<32>{});
}

#ifdef DEEP_TEMPLATE_RECURSIVE_BUILD
TEST_F(ALUTests, ALU64)
{
    ALUOverflowTest(std::make_index_sequence<64>{});
}
#endif
} // namespace component
