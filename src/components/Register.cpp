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
#include <ComponentFactory.hpp>

namespace component
{
REGISTER_COMPONENT(DFlipFlop);
REGISTER_COMPONENT(Bit);
REGISTER_COMPONENT(Register<1>, 1); // for test
REGISTER_COMPONENT(Register<4>, 4);
REGISTER_COMPONENT(Register<8>, 8);
REGISTER_COMPONENT(Register<16>, 16);
REGISTER_COMPONENT(Register<32>, 32);
#ifdef DEEP_TEMPLATE_RECURSIVE_BUILD
REGISTER_COMPONENT(Register<64>, 64);
#endif
} // namespace component
