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
#include <Bus.hpp>

#include <gtest/gtest.h>

namespace component
{

class BusTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(BusTests, initialize)
{
    auto bus1 = ComponentFactory::create("Bus1", "bus1");
    bus1->initialize();
    auto bus2 = Bus<2>::create("bus2");
    bus2->initialize();
    auto bus4 = ComponentFactory::create("Bus4", "bus4");
    bus4->initialize();
    auto bus8 = ComponentFactory::create("Bus8", "bus8");
    bus8->initialize();
    auto bus16 = ComponentFactory::create("Bus16", "bus16");
    bus16->initialize();
    auto bus32 = ComponentFactory::create("Bus32", "bus32");
    bus32->initialize();
    auto bus64 = ComponentFactory::create("Bus64", "bus64");
    bus64->initialize();
}

} // namespace component
