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

namespace component
{
REGISTER_COMPONENT(BitAdder);
REGISTER_COMPONENT(BinaryAdder<1>, 1); // for test
REGISTER_COMPONENT(BinaryAdder<4>, 4);
REGISTER_COMPONENT(BinaryAdder<8>, 8);
REGISTER_COMPONENT(BinaryAdder<16>, 16);
REGISTER_COMPONENT(BinaryAdder<32>, 32);
REGISTER_COMPONENT(BinaryAdder<64>, 64);
REGISTER_COMPONENT(ALU<1>, alu1); // for test
REGISTER_COMPONENT(ALU<4>, alu4);
REGISTER_COMPONENT(ALU<8>, alu8);
REGISTER_COMPONENT(ALU<16>, alu16);
REGISTER_COMPONENT(ALU<32>, alu32);
REGISTER_COMPONENT(ALU<64>, alu64);
} // namespace component
