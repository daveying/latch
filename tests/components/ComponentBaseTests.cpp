#include <ComponentBase.hpp>
#include <ComponentFactory.hpp>
#include <GateComponents.hpp>

#include <gtest/gtest.h>

namespace component
{

class ComponentBaseTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::ANDGateComponent>("ANDGateComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NANDGateComponent>("NANDGateComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NORGateComponent>("NORGateComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NORGateDelayedComponent>("NORGateDelayedComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::ORGateComponent>("ORGateComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NOTGateComponent>("NOTGateComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::NOTGateDelayedComponent>("NOTGateDelayedComponent"));
        ASSERT_NO_THROW(ComponentFactory::registerPrecompiledComponent<component::BusBufferComponent>("BusBufferComponent"));
    }

    void TearDown() override
    {
        ComponentFactory::clear();
    }

    void registerOneBitDescription()
    {
        ComponentDescription description;
        description.type = "OneBit";
        description.pins.push_back({"Load", PinDirection::INPUT});  // -1,0
        description.pins.push_back({"D", PinDirection::INPUT});     // -1,1
        description.pins.push_back({"Clock", PinDirection::INPUT}); // -1,2
        description.pins.push_back({"Q", PinDirection::OUTPUT});    // -1,3

        description.subcomponents.push_back({"dataInvertor", "NOTGateComponent"});          // 0
        description.subcomponents.push_back({"feedbackAnd", "ANDGateComponent"});           // 1
        description.subcomponents.push_back({"dataAnd", "ANDGateComponent"});               // 2
        description.subcomponents.push_back({"dataSelector", "ORGateComponent"});           // 3
        description.subcomponents.push_back({"dff.invertor", "NOTGateDelayedComponent"});   // 4
        description.subcomponents.push_back({"dff.pulseGate", "ANDGateComponent"});         // 5
        description.subcomponents.push_back({"dff.notGate", "NOTGateComponent"});           // 6
        description.subcomponents.push_back({"dff.andGate0", "ANDGateComponent"});          // 7
        description.subcomponents.push_back({"dff.andGate1", "ANDGateComponent"});          // 8
        description.subcomponents.push_back({"dff.norGate0", "NORGateDelayedComponent"});   // 9
        description.subcomponents.push_back({"dff.norGate1", "NORGateComponent"});          // 10

        description.connections.push_back({.src = {-1, 0}, .dest = {0,  0}});  // Load            -> dataInvertor.0
        description.connections.push_back({.src = {-1, 0}, .dest = {2,  0}});  // Load            -> dataAnd.0
        description.connections.push_back({.src = {-1, 1}, .dest = {2,  1}});  // D               -> dataAnd.1
        description.connections.push_back({.src = {-1, 2}, .dest = {4,  0}});  // Clock           -> dff.invertor.0
        description.connections.push_back({.src = {-1, 2}, .dest = {5,  0}});  // Clock           -> dff.pulseGate.0
        description.connections.push_back({.src = {0,  1}, .dest = {1,  1}});  // dataInvertor.1  -> feedbackAnd.1
        description.connections.push_back({.src = {4,  1}, .dest = {5,  1}});  // dff.invertor.1  -> dff.pulseGate.1
        description.connections.push_back({.src = {1,  2}, .dest = {3,  0}});  // feedbackAnd.2   -> dataSelector.0
        description.connections.push_back({.src = {2,  2}, .dest = {3,  1}});  // dataAnd.2       -> dataSelector.1
        description.connections.push_back({.src = {3,  2}, .dest = {7,  0}});  // dataSelector.2  -> dff.andGate0.0
        description.connections.push_back({.src = {3,  2}, .dest = {6,  0}});  // dataSelector.2  -> dff.notGate.0
        description.connections.push_back({.src = {6,  1}, .dest = {8,  1}});  // dff.notGate.1   -> dff.andGate1.1
        description.connections.push_back({.src = {5,  2}, .dest = {7,  1}});  // dff.pulseGata.2 -> dff.andGate0.1
        description.connections.push_back({.src = {5,  2}, .dest = {8,  0}});  // dff.pulseGata.2 -> dff.andGate1.0
        description.connections.push_back({.src = {7,  2}, .dest = {9,  0}});  // dff.andGate0.2  -> dff.norGate0.0
        description.connections.push_back({.src = {8,  2}, .dest = {10, 1}});  // dff.andGate1.2  -> dff.norGate1.1
        description.connections.push_back({.src = {10, 2}, .dest = {9,  1}});  // dff.norGate1.2  -> dff.norGate0.1
        description.connections.push_back({.src = {9,  2}, .dest = {10, 0}});  // dff.norGate0.2  -> dff.norGate1.0
        description.connections.push_back({.src = {10, 2}, .dest = {1,  0}});  // dff.norGate1.2  -> feedbackAnd.0
        description.connections.push_back({.src = {10, 2}, .dest = {-1, 3}});  // dff.norGate1.2  -> Q
        ComponentFactory::registerCustomComponents(description);
    }

    void registerFourBitDescription()
    {
        ComponentDescription description;
        description.type = "FourBit";
        description.pins.push_back({"Load", PinDirection::INPUT});  // -1,0
        description.pins.push_back({"D0", PinDirection::INPUT});    // -1,1
        description.pins.push_back({"D1", PinDirection::INPUT});    // -1,2
        description.pins.push_back({"D2", PinDirection::INPUT});    // -1,3
        description.pins.push_back({"D3", PinDirection::INPUT});    // -1,4
        description.pins.push_back({"Clock", PinDirection::INPUT}); // -1,5
        description.pins.push_back({"Q0", PinDirection::OUTPUT});   // -1,6
        description.pins.push_back({"Q1", PinDirection::OUTPUT});   // -1,7
        description.pins.push_back({"Q2", PinDirection::OUTPUT});   // -1,8
        description.pins.push_back({"Q3", PinDirection::OUTPUT});   // -1,9

        description.subcomponents.push_back({"bit0", "OneBit"}); // 0
        description.subcomponents.push_back({"bit1", "OneBit"}); // 1
        description.subcomponents.push_back({"bit2", "OneBit"}); // 2
        description.subcomponents.push_back({"bit3", "OneBit"}); // 3

        description.connections.push_back({.src = {-1, 0}, .dest = {0,  0}});  // Load  -> bit0.Load
        description.connections.push_back({.src = {-1, 0}, .dest = {1,  0}});  // Load  -> bit1.Load
        description.connections.push_back({.src = {-1, 0}, .dest = {2,  0}});  // Load  -> bit2.Load
        description.connections.push_back({.src = {-1, 0}, .dest = {3,  0}});  // Load  -> bit3.Load
        description.connections.push_back({.src = {-1, 5}, .dest = {0,  2}});  // Clock -> bit0.Clock
        description.connections.push_back({.src = {-1, 5}, .dest = {1,  2}});  // Clock -> bit1.Clock
        description.connections.push_back({.src = {-1, 5}, .dest = {2,  2}});  // Clock -> bit2.Clock
        description.connections.push_back({.src = {-1, 5}, .dest = {3,  2}});  // Clock -> bit3.Clock

        description.connections.push_back({.src = {-1, 1}, .dest = {0,  1}});  // D0    -> bit0.D
        description.connections.push_back({.src = {-1, 2}, .dest = {1,  1}});  // D1    -> bit1.D
        description.connections.push_back({.src = {-1, 3}, .dest = {2,  1}});  // D2    -> bit2.D
        description.connections.push_back({.src = {-1, 4}, .dest = {3,  1}});  // D3    -> bit3.D

        description.connections.push_back({.src = {0, 3}, .dest = {-1,  6}});  // bit0.Q -> Q0
        description.connections.push_back({.src = {1, 3}, .dest = {-1,  7}});  // bit1.Q -> Q1
        description.connections.push_back({.src = {2, 3}, .dest = {-1,  8}});  // bit2.Q -> Q2
        description.connections.push_back({.src = {3, 3}, .dest = {-1,  9}});  // bit3.Q -> Q3
        ComponentFactory::registerCustomComponents(description);
    }
};

TEST_F(ComponentBaseTests, OneBit)
{
    registerOneBitDescription();
    std::unique_ptr<IComponent> oneBit;
    ASSERT_NO_THROW(oneBit = ComponentFactory::create("OneBit", "oneBit"));
    ASSERT_NO_THROW(oneBit->initialize());

    auto Load  = oneBit->pin(0);
    auto D     = oneBit->pin(1);
    auto Clock = oneBit->pin(2);
    auto Q     = oneBit->pin(3);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // D -> High, Load Low
    D->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q->value(), gate::PinState::Low);
    EXPECT_EQ(Clock->value(), gate::PinState::Low);

    // D High, Load High
    Load->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q->value(), gate::PinState::High);
}

TEST_F(ComponentBaseTests, FourBit)
{
    registerOneBitDescription();
    registerFourBitDescription();

    std::unique_ptr<IComponent> fourBit;
    ASSERT_NO_THROW(fourBit = ComponentFactory::create("FourBit", "fourBit"));
    ASSERT_NO_THROW(fourBit->initialize());

    auto Load  = fourBit->pin(0);
    auto D0    = fourBit->pin(1);
    auto D1    = fourBit->pin(2);
    auto D2    = fourBit->pin(3);
    auto D3    = fourBit->pin(4);
    auto Clock = fourBit->pin(5);
    auto Q0    = fourBit->pin(6);
    auto Q1    = fourBit->pin(7);
    auto Q2    = fourBit->pin(8);
    auto Q3    = fourBit->pin(9);

    sched::waitTillSteady();

    // D Low, Load Low
    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);

    // D -> High, Load Low
    D0->value(gate::PinState::High);
    D1->value(gate::PinState::Low);
    D2->value(gate::PinState::High);
    D3->value(gate::PinState::Low);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(2, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();

    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);
    EXPECT_EQ(Clock->value(), gate::PinState::Low);

    // D High, Load High
    Load->value(gate::PinState::High);
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), gate::PinState::Low);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::Low);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);

    // Clock -> pulse
    sched::addEvent(1, sched::Event::create("Clock goes high", [&](sched::Timestamp) { Clock->value(gate::PinState::High); }));
    sched::addEvent(3, sched::Event::create("Clock goes low", [&](sched::Timestamp) { Clock->value(gate::PinState::Low); }));
    sched::waitTillSteady();
    EXPECT_EQ(Q0->value(), gate::PinState::High);
    EXPECT_EQ(Q1->value(), gate::PinState::Low);
    EXPECT_EQ(Q2->value(), gate::PinState::High);
    EXPECT_EQ(Q3->value(), gate::PinState::Low);
}
} // namespace component
