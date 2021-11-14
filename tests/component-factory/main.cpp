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

