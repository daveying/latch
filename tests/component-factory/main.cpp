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
#include <GateComponents.hpp>

#include <gtest/gtest.h>

using component::ComponentFactory;

TEST(ComponentFactory, AddPreCompiledComponents)
{
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::ANDGateComponent>("ANDGateComponent"));
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NANDGateComponent>("NANDGateComponent"));
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NORGateComponent>("NORGateComponent"));
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::ORGateComponent>("ORGateComponent"));
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NOTGateComponent>("NOTGateComponent"));
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NOTGateDelayedComponent>("NOTGateDelayedComponent"));
    ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::BusBufferComponent>("BusBufferComponent"));

    auto andGateComp        = ComponentFactory::create("ANDGateComponent", "andGateComp");
    auto nandGateComp       = ComponentFactory::create("NANDGateComponent", "nandGateComp");
    auto norGateComp        = ComponentFactory::create("NORGateComponent", "norGateComp");
    auto orGateComp         = ComponentFactory::create("ORGateComponent", "orGateComp");
    auto notGateComp        = ComponentFactory::create("NOTGateComponent", "notGateComp");
    auto notGateDelayedComp = ComponentFactory::create("NOTGateDelayedComponent", "notGateDelayedComp");
    auto BusBufferComp      = ComponentFactory::create("BusBufferComponent", "BusBufferComp");

    EXPECT_STREQ(andGateComp->name().c_str(), "andGateComp");
}

