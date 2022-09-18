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

#include <ComponentFactory.hpp>
#include <ComponentDescription.hpp>
#include <Bus.hpp>
#include <ALU.hpp>
#include <Register.hpp>
#include <IScheduler.hpp>

#include <gtest/gtest.h>

namespace component
{

template <size_t BITS>
class ALU_A_B : public ComponentBase
{
public:
    static_assert(BITS >= 1, "BITS must >= 1");
    static const char* Name()
    {
        static std::string name{"ALU_A_B" + std::to_string(BITS)};
        return name.c_str();
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN("AI", ForwardInputPin),
            DEFINE_PIN("AO", ForwardInputPin),
            DEFINE_PIN("BI", ForwardInputPin),
            DEFINE_PIN("BO", ForwardInputPin),
            DEFINE_PIN("EO", ForwardInputPin),
            DEFINE_PIN("SUB", ForwardInputPin),
            DEFINE_PIN_ARRAY("BusI", ForwardInputPin, BITS),  // for test
            DEFINE_PIN_ARRAY("BusO", ForwardOutputPin, BITS)  // for test
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("ALU", ALU<BITS>),
            DEFINE_SUBCOMPONENT("A", Register<BITS>),
            DEFINE_SUBCOMPONENT("B", Register<BITS>),
            DEFINE_SUBCOMPONENT_ARRAY("Aout", BusBufferComponent, BITS),
            DEFINE_SUBCOMPONENT_ARRAY("Bout", BusBufferComponent, BITS),
            DEFINE_SUBCOMPONENT("Bus", Bus<BITS>)
        );
    }
    static constexpr auto Connections()
    {
        return std::make_tuple(
            CONNECT("AI", "A.Load"),
            CONNECT_MULTICAST_COMPONENT("AO", "Aout.in1", BITS),
            CONNECT("BI", "B.Load"),
            CONNECT_MULTICAST_COMPONENT("BO", "Bout.in1", BITS),
            CONNECT("EO", "ALU.EO"),
            CONNECT("SUB", "ALU.SUB"),
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("ALU.Sum", "Bus.I", BITS),
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("A.Q", "ALU.A", BITS),
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("B.Q", "ALU.B", BITS),
            CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY("A.Q", "Aout.in0", BITS),
            CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY("B.Q", "Bout.in0", BITS),
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("Aout.out0", "Bus.I", BITS),
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("Bout.out0", "Bus.I", BITS),
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("Bus.O", "A.D", BITS),
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("Bus.O", "B.D", BITS),
            CONNECT("Clock", "A.Clock"),
            CONNECT("Clock", "B.Clock"),
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("Bus.O", "BusO", BITS), // for test
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("BusI", "Bus.I", BITS)  // for test
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<ALU_A_B>(name);
    }
    ALU_A_B(const std::string& name)
        : ComponentBase(detail::getDescription<ALU_A_B>(), name)
    {}
    virtual ~ALU_A_B() {}
};

class BusTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
    template <size_t... I>
    void getPins(IComponent* aluAB,
            IPin*& Clock,
            IPin*& AI,
            IPin*& AO,
            IPin*& BI,
            IPin*& BO,
            IPin*& EO,
            IPin*& SUB,
            std::vector<IPin*>& BusI,
            std::vector<IPin*>& BusO,
            const std::index_sequence<I...>& s)
    {
        constexpr size_t BITS = sizeof...(I);
        Clock = aluAB->pin(0);
        AI    = aluAB->pin(1);
        AO    = aluAB->pin(2);
        BI    = aluAB->pin(3);
        BO    = aluAB->pin(4);
        EO    = aluAB->pin(5);
        SUB   = aluAB->pin(6);
        std::array<IPin*, BITS> busI = {aluAB->pin(7 + I)...};
        std::array<IPin*, BITS> busO = {aluAB->pin(7 + BITS + I)...};
        for (size_t i = 0; i < BITS; ++i)
        {
            BusI.push_back(busI[i]);
        }
        for (size_t i = 0; i < BITS; ++i)
        {
            BusO.push_back(busO[i]);
        }
    }
    void setVal(uint64_t val, std::vector<IPin*>& pins, uint64_t bits)
    {
        for (uint64_t i = 0; i < bits; ++i)
        {
            uint64_t mask = 1ull << i;
            if ((mask & val) > 0)
            {
                pins[i]->value(PinState::High);
            }
            else
            {
                pins[i]->value(PinState::Low);
            }
        }
    }
    void checkVal(uint64_t val, std::vector<IPin*>& pins, uint64_t bits)
    {
        for (uint64_t i = 0; i < bits; ++i)
        {
            uint64_t mask = 1ull << i;
            if ((mask & val) > 0)
            {
                ASSERT_EQ(pins[i]->value(), PinState::High);
            }
            else
            {
                ASSERT_EQ(pins[i]->value(), PinState::Low);
            }
        }
    }
    template <size_t BITS>
    void ALU_A_B_addTest()
    {
        auto aluAB = ALU_A_B<BITS>::create("aluAB" + std::to_string(BITS));
        aluAB->initialize();
        IPin* Clock;
        IPin* AI;
        IPin* AO;
        IPin* BI;
        IPin* BO;
        IPin* EO;
        IPin* SUB;
        std::vector<IPin*> BusI;
        std::vector<IPin*> BusO;
        getPins(aluAB.get(), Clock, AI, AO, BI, BO, EO, SUB, BusI, BusO, std::make_index_sequence<BITS>{});
        ASSERT_EQ(PinState::Low, Clock->value());
        ASSERT_EQ(PinState::Low, AI->value());
        ASSERT_EQ(PinState::Low, AO->value());
        ASSERT_EQ(PinState::Low, BI->value());
        ASSERT_EQ(PinState::Low, BO->value());
        ASSERT_EQ(PinState::Low, EO->value());
        ASSERT_EQ(PinState::Low, SUB->value());
        for (size_t i = 0; i < BITS; ++i)
        {
            ASSERT_EQ(PinState::Low, BusI[i]->value());
            ASSERT_EQ(PinState::Low, BusO[i]->value());
        }

        setVal(111ull, BusI, BITS);
        AI->value(PinState::High);
        sched::addEvent(1, sched::Event::create("Clock goes High", [&](sched::Timestamp) {
            Clock->value(PinState::High);
        }));
        sched::waitTillSteady();
        AI->value(PinState::Low);
        sched::waitTillSteady();
        EO->value(PinState::High);
        sched::waitTillSteady();
        checkVal(111ull, BusO, BITS);
        sched::addEvent(1, sched::Event::create("Clock goes Low", [&](sched::Timestamp) {
            Clock->value(PinState::Low);
        }));
        sched::waitTillSteady();
        checkVal(111ull, BusO, BITS);

        BI->value(PinState::High);

        // each clock pulse will increament B and and BusI/O by 111
        for (size_t i = 0; i < 2 * BITS; ++i)
        {
            sched::addEvent(1, sched::Event::create("Clock goes High", [&](sched::Timestamp) {
                Clock->value(PinState::High);
            }));
            sched::addEvent(2, sched::Event::create("Clock goes Low", [&](sched::Timestamp) {
                Clock->value(PinState::Low);
            }));
            sched::waitTillSteady();
            checkVal(222ull + i * 111ull, BusO, BITS);
        }
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
#ifdef DEEP_TEMPLATE_RECURSIVE_BUILD
    auto bus64 = ComponentFactory::create("Bus64", "bus64");
    bus64->initialize();
#endif
}

TEST_F(BusTests, ALU_A_B1)
{
    ALU_A_B_addTest<1>();
}
TEST_F(BusTests, ALU_A_B4)
{
    ALU_A_B_addTest<4>();
}
TEST_F(BusTests, ALU_A_B8)
{
    ALU_A_B_addTest<8>();
}
TEST_F(BusTests, ALU_A_B16)
{
    ALU_A_B_addTest<16>();
}
TEST_F(BusTests, ALU_A_B32)
{
    ALU_A_B_addTest<32>();
}
#ifdef DEEP_TEMPLATE_RECURSIVE_BUILD
TEST_F(BusTests, ALU_A_B64)
{
    ALU_A_B_addTest<64>();
}
#endif

} // namespace component
