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

#define EXPECT_THROW_WITH_MESSAGE(stmt, etype, whatstring) EXPECT_THROW( \
    try {                                                                \
        stmt;                                                            \
    } catch (const etype& ex) {                                          \
        EXPECT_EQ(std::string(ex.what()), whatstring);                   \
        throw;                                                           \
    }                                                                    \
    , etype)

using component::ComponentFactory;

TEST(ComponentFactory, AddPreCompiledComponents)
{
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::ANDGateComponent>(),
            std::runtime_error,
            "Precompiled component type: ANDGateComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::NANDGateComponent>(),
            std::runtime_error,
            "Precompiled component type: NANDGateComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::NORGateComponent>(),
            std::runtime_error,
            "Precompiled component type: NORGateComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::ORGateComponent>(),
            std::runtime_error,
            "Precompiled component type: ORGateComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::XORGateComponent>(),
            std::runtime_error,
            "Precompiled component type: XORGateComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::NOTGateComponent>(),
            std::runtime_error,
            "Precompiled component type: NOTGateComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::NOTGateDelayedComponent>(),
            std::runtime_error,
            "Precompiled component type: NOTGateDelayedComponent has already been registered."
            );
    EXPECT_THROW_WITH_MESSAGE(
            ComponentFactory::registerPrecompiledComponent<component::BusBufferComponent>(),
            std::runtime_error,
            "Precompiled component type: BusBufferComponent has already been registered."
            );

    auto andGateComp        = ComponentFactory::create("ANDGateComponent", "andGateComp");
    auto nandGateComp       = ComponentFactory::create("NANDGateComponent", "nandGateComp");
    auto norGateComp        = ComponentFactory::create("NORGateComponent", "norGateComp");
    auto orGateComp         = ComponentFactory::create("ORGateComponent", "orGateComp");
    auto xorGateComp        = ComponentFactory::create("XORGateComponent", "xorGateComp");
    auto notGateComp        = ComponentFactory::create("NOTGateComponent", "notGateComp");
    auto notGateDelayedComp = ComponentFactory::create("NOTGateDelayedComponent", "notGateDelayedComp");
    auto BusBufferComp      = ComponentFactory::create("BusBufferComponent", "BusBufferComp");

    EXPECT_STREQ(andGateComp->name().c_str(), "andGateComp");

    component::JSON dump = ComponentFactory::dump();
    std::cout << dump.dump(2) << std::endl;
}

