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

#ifndef PROGRAM_COUNTER_HPP__
#define PROGRAM_COUNTER_HPP__

#include <IComponent.hpp>
#include <ComponentBase.hpp>
#include <ComponentDescription.hpp>
#include <GateComponents.hpp>

namespace component
{

class MSJKFlipFlop : public ComponentBase
{
public:
    static constexpr const char* Name()
    {
        return "MSJKFlipFlop";
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN("J", ForwardInputPin),
            DEFINE_PIN("K", ForwardInputPin),
            DEFINE_PIN("Q", ForwardOutputPin),
            DEFINE_PIN("Qc", ForwardOutputPin)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("and0", ANDGate3Component),
            DEFINE_SUBCOMPONENT("and1", ANDGate3Component),
            DEFINE_SUBCOMPONENT("nor0", NORGateComponent),
            DEFINE_SUBCOMPONENT("nor1", NORGateComponent),
            DEFINE_SUBCOMPONENT("and2", ANDGateComponent),
            DEFINE_SUBCOMPONENT("and3", ANDGateComponent),
            DEFINE_SUBCOMPONENT("nor3", NORGateComponent), // reverse the order to make Q Low after initialize
            DEFINE_SUBCOMPONENT("nor2", NORGateComponent),
            DEFINE_SUBCOMPONENT("not", NOTGateComponent)
        );
    }
    static constexpr auto Connections()
    {
        return std::make_tuple(
            CONNECT("J", "and0.in1"),
            CONNECT("K", "and1.in1"),
            CONNECT("Clock", "and0.in2"),
            CONNECT("Clock", "and1.in0"),
            CONNECT("Clock", "not.in0"),
            CONNECT("and0.out0", "nor0.in0"),
            CONNECT("and1.out0", "nor1.in1"),
            CONNECT("nor0.out0", "nor1.in0"),
            CONNECT("nor1.out0", "nor0.in1"),
            CONNECT("nor0.out0", "and2.in0"),
            CONNECT("nor1.out0", "and3.in1"),
            CONNECT("not.out0", "and2.in1"),
            CONNECT("not.out0", "and3.in0"),
            CONNECT("and2.out0", "nor2.in0"),
            CONNECT("and3.out0", "nor3.in1"),
            CONNECT("nor3.out0", "nor2.in1"),
            CONNECT("nor2.out0", "nor3.in0"),
            CONNECT("nor2.out0", "and1.in2"),
            CONNECT("nor3.out0", "and0.in0"),
            CONNECT("nor2.out0", "Q"),
            CONNECT("nor3.out0", "Qc")
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<MSJKFlipFlop>(name);
    }
    MSJKFlipFlop(const std::string& name)
        : ComponentBase(detail::getDescription<MSJKFlipFlop>(), name)
    {}
    virtual ~MSJKFlipFlop() {}
};

template <size_t BITS>
class BinaryCounter : public ComponentBase
{
public:
    static const char* Name()
    {
        static std::string name{"BinaryCounter" + std::to_string(BITS)};
        return name.c_str();
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN_ARRAY("C", ForwardInputPin, BITS)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT_ARRAY("msJK", MSJKFlipFlop, BITS)
        );
    }
    static constexpr auto Connections()
    {
        auto conn0 = std::make_tuple(
            CONNECT("Clock", "msJK[0].Clock"),
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("msJK.Q", "C", BITS)
        );
        auto conn1 = connectCarryPins(std::make_index_sequence<BITS - 1>{});
        return detail::cmb(conn0, conn1);
    }
    virtual void initialize() override
    {
        ComponentBase::initialize();
        // J & K set to high and never changes
        for (size_t i = 0; i < BITS; ++i)
        {
            m_subcomponents[i]->pin(1)->value(PinState::High);
            m_subcomponents[i]->pin(2)->value(PinState::High);
        }
        sched::waitTillSteady();
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<BinaryCounter>(name);
    }
    BinaryCounter(const std::string& name)
        : ComponentBase(detail::getDescription<BinaryCounter>(), name)
    {}
    virtual ~BinaryCounter() {}
private:
    template <size_t... I>
    static constexpr auto connectCarryPins(const std::index_sequence<I...>&)
    {
        return std::make_tuple(
            CONNECT(sp("msJK[", I, "].Q"), sp("msJK[", I + 1, "].Clock"))...
        );
    }
};

class MSJKFlipFlopWithPresetClear : public ComponentBase
{
public:
    static constexpr const char* Name()
    {
        return "MSJKFlipFlopWithPresetClear";
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN("J", ForwardInputPin),
            DEFINE_PIN("K", ForwardInputPin),
            DEFINE_PIN("PR", ForwardInputPin),
            DEFINE_PIN("CLR", ForwardInputPin),
            DEFINE_PIN("Q", ForwardOutputPin),
            DEFINE_PIN("Qc", ForwardOutputPin)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("brkAnd0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("brkAnd1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("brkAnd2", ANDGateComponent),
            DEFINE_SUBCOMPONENT("nor", NORGateComponent),
            DEFINE_SUBCOMPONENT("and0", ANDGate3Component),
            DEFINE_SUBCOMPONENT("and1", ANDGate3Component),
            DEFINE_SUBCOMPONENT("nor0", NORGate3Component),
            DEFINE_SUBCOMPONENT("nor1", NORGate3Component),
            DEFINE_SUBCOMPONENT("and2", ANDGateComponent),
            DEFINE_SUBCOMPONENT("and3", ANDGateComponent),
            DEFINE_SUBCOMPONENT("nor3", NORGateComponent), // reverse the order to make Q Low after initialize
            DEFINE_SUBCOMPONENT("nor2", NORGateComponent),
            DEFINE_SUBCOMPONENT("not", NOTGateComponent)
        );
    }
    static constexpr auto Connections()
    {
        return std::make_tuple(
            CONNECT("J", "brkAnd0.in0"),
            CONNECT("K", "brkAnd2.in0"),
            CONNECT("Clock", "brkAnd1.in0"),
            CONNECT("CLR", "nor.in0"),
            CONNECT("CLR", "nor1.in2"),
            CONNECT("PR", "nor.in1"),
            CONNECT("PR", "nor0.in0"),
            CONNECT("nor.out0", "brkAnd0.in1"),
            CONNECT("nor.out0", "brkAnd1.in1"),
            CONNECT("nor.out0", "brkAnd2.in1"),
            CONNECT("brkAnd0.out0", "and0.in1"),
            CONNECT("brkAnd2.out0", "and1.in1"),
            CONNECT("brkAnd1.out0", "and0.in2"),
            CONNECT("brkAnd1.out0", "and1.in0"),
            CONNECT("brkAnd1.out0", "not.in0"),
            CONNECT("and0.out0", "nor0.in1"),
            CONNECT("and1.out0", "nor1.in1"),
            CONNECT("nor0.out0", "nor1.in0"),
            CONNECT("nor1.out0", "nor0.in2"),
            CONNECT("nor0.out0", "and2.in0"),
            CONNECT("nor1.out0", "and3.in1"),
            CONNECT("not.out0", "and2.in1"),
            CONNECT("not.out0", "and3.in0"),
            CONNECT("and2.out0", "nor2.in0"),
            CONNECT("and3.out0", "nor3.in1"),
            CONNECT("nor3.out0", "nor2.in1"),
            CONNECT("nor2.out0", "nor3.in0"),
            CONNECT("nor2.out0", "and1.in2"),
            CONNECT("nor3.out0", "and0.in0"),
            CONNECT("nor2.out0", "Q"),
            CONNECT("nor3.out0", "Qc")
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<MSJKFlipFlopWithPresetClear>(name);
    }
    MSJKFlipFlopWithPresetClear(const std::string& name)
        : ComponentBase(detail::getDescription<MSJKFlipFlopWithPresetClear>(), name)
    {}
    virtual ~MSJKFlipFlopWithPresetClear() {}
};

// 4 bits synchronous binary counter with clear load and enable
// clear is asynchronous
class SynchronousBinaryCounter : public ComponentBase
{
public:
    static constexpr const size_t BITS = 4;
    static constexpr const char* Name()
    {
        return "SynchronousBinaryCounter";
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            // control pins
            DEFINE_PIN("CLK", ForwardInputPin),         // raising edge triggered
            DEFINE_PIN("CLR", ForwardInputPin),         // high level active
            DEFINE_PIN("LOAD", ForwardInputPin),        // high level active
            DEFINE_PIN("ENABLE", ForwardInputPin),      // high level active
            DEFINE_PIN_ARRAY("D", ForwardInputPin, BITS),
            DEFINE_PIN_ARRAY("Q", ForwardOutputPin, BITS),
            DEFINE_PIN("RC", ForwardOutputPin)          // ripple carry output
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("clkNot", NOTGateUnitInertialComponent),
            DEFINE_SUBCOMPONENT("enNot", NOTGateComponent),
            DEFINE_SUBCOMPONENT("ldClrOr", ORGateComponent),
            DEFINE_SUBCOMPONENT("enLdNor", NORGateComponent),
            DEFINE_SUBCOMPONENT("k0And0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("k0And1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j0And0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j0And1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("k1And0", ANDGate3Component),
            DEFINE_SUBCOMPONENT("k1And1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j1And0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j1And1", ANDGate3Component),
            DEFINE_SUBCOMPONENT("k2And0", ANDGate4Component),
            DEFINE_SUBCOMPONENT("k2And1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j2And0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j2And1", ANDGate4Component),
            DEFINE_SUBCOMPONENT("k3And0", ANDGate5Component),
            DEFINE_SUBCOMPONENT("k3And1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j3And0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("j3And1", ANDGate5Component),
            DEFINE_SUBCOMPONENT("k0Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j0Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j0Not", NOTGateComponent),
            DEFINE_SUBCOMPONENT("k1Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j1Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j1Not", NOTGateComponent),
            DEFINE_SUBCOMPONENT("k2Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j2Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j2Not", NOTGateComponent),
            DEFINE_SUBCOMPONENT("k3Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j3Or", ORGateComponent),
            DEFINE_SUBCOMPONENT("j3Not", NOTGateComponent),
            DEFINE_SUBCOMPONENT_ARRAY("jk", MSJKFlipFlopWithPresetClear, BITS),
            DEFINE_SUBCOMPONENT("rcNor", NORGate4Component)
        );
    }
    static constexpr auto Connections()
    {
        return std::make_tuple(
            // connections for CLK
            CONNECT("CLK", "clkNot.in0"),
            CONNECT_MULTICAST_COMPONENT("clkNot.out0", "jk.Clock", BITS),
            // connections for CLR
            CONNECT_MULTICAST_COMPONENT("CLR", "jk.CLR", BITS),
            // connections for ENABLE CLR control logic
            CONNECT("ENABLE", "enNot.in0"),
            CONNECT("enNot.out0", "enLdNor.in1"),
            CONNECT("CLR", "ldClrOr.in0"),
            // connections for LOAD control lines
            CONNECT("LOAD", "ldClrOr.in1"),
            CONNECT("ldClrOr.out0", "enLdNor.in0"),
            CONNECT("ldClrOr.out0", "k0And1.in0"),
            CONNECT("ldClrOr.out0", "j0And0.in1"),
            CONNECT("D[0]", "j0And0.in0"),
            CONNECT("ldClrOr.out0", "k1And1.in0"),
            CONNECT("ldClrOr.out0", "j1And0.in1"),
            CONNECT("D[1]", "j1And0.in0"),
            CONNECT("ldClrOr.out0", "k2And1.in0"),
            CONNECT("ldClrOr.out0", "j2And0.in1"),
            CONNECT("D[2]", "j2And0.in0"),
            CONNECT("ldClrOr.out0", "k3And1.in0"),
            CONNECT("ldClrOr.out0", "j3And0.in1"),
            CONNECT("D[3]", "j3And0.in0"),
            // connections for ENABLE control lines
            CONNECT("enLdNor.out0", "k0And0.in1"),
            CONNECT("enLdNor.out0", "j0And1.in0"),
            CONNECT("enLdNor.out0", "k1And0.in2"),
            CONNECT("enLdNor.out0", "j1And1.in0"),
            CONNECT("enLdNor.out0", "k2And0.in3"),
            CONNECT("enLdNor.out0", "j2And1.in0"),
            CONNECT("enLdNor.out0", "k3And0.in4"),
            CONNECT("enLdNor.out0", "j3And1.in0"),
            // connections for Qs' adding
            CONNECT("jk[0].Q", "k0And0.in0"),
            CONNECT("jk[0].Q", "k1And0.in1"),
            CONNECT("jk[0].Q", "j1And1.in1"),
            CONNECT("jk[0].Q", "k2And0.in1"),
            CONNECT("jk[0].Q", "j2And1.in1"),
            CONNECT("jk[0].Q", "k3And0.in1"),
            CONNECT("jk[0].Q", "j3And1.in1"),
            CONNECT("jk[1].Q", "k1And0.in0"),
            CONNECT("jk[1].Q", "k2And0.in2"),
            CONNECT("jk[1].Q", "j2And1.in2"),
            CONNECT("jk[1].Q", "k3And0.in2"),
            CONNECT("jk[1].Q", "j3And1.in2"),
            CONNECT("jk[2].Q", "k2And0.in0"),
            CONNECT("jk[2].Q", "k3And0.in3"),
            CONNECT("jk[2].Q", "j3And1.in3"),
            CONNECT("jk[3].Q", "k3And0.in0"),
            // connections for Qcs'
            CONNECT("jk[0].Qc", "j0And1.in1"),
            CONNECT("jk[1].Qc", "j1And1.in2"),
            CONNECT("jk[2].Qc", "j2And1.in3"),
            CONNECT("jk[3].Qc", "j3And1.in4"),
            CONNECT("jk[0].Qc", "rcNor.in3"),
            CONNECT("jk[1].Qc", "rcNor.in2"),
            CONNECT("jk[2].Qc", "rcNor.in1"),
            CONNECT("jk[3].Qc", "rcNor.in0"),
            CONNECT("rcNor.out0", "RC"),
            // connections for selectors
            CONNECT("k0And0.out0", "k0Or.in0"),
            CONNECT("k0And1.out0", "k0Or.in1"),
            CONNECT("j0And0.out0", "j0Or.in0"),
            CONNECT("j0And1.out0", "j0Or.in1"),
            CONNECT("k1And0.out0", "k1Or.in0"),
            CONNECT("k1And1.out0", "k1Or.in1"),
            CONNECT("j1And0.out0", "j1Or.in0"),
            CONNECT("j1And1.out0", "j1Or.in1"),
            CONNECT("k2And0.out0", "k2Or.in0"),
            CONNECT("k2And1.out0", "k2Or.in1"),
            CONNECT("j2And0.out0", "j2Or.in0"),
            CONNECT("j2And1.out0", "j2Or.in1"),
            CONNECT("k3And0.out0", "k3Or.in0"),
            CONNECT("k3And1.out0", "k3Or.in1"),
            CONNECT("j3And0.out0", "j3Or.in0"),
            CONNECT("j3And1.out0", "j3Or.in1"),
            // connections for J & K
            CONNECT("k0Or.out0", "jk[0].K"),
            CONNECT("j0Or.out0", "jk[0].J"),
            CONNECT("k1Or.out0", "jk[1].K"),
            CONNECT("j1Or.out0", "jk[1].J"),
            CONNECT("k2Or.out0", "jk[2].K"),
            CONNECT("j2Or.out0", "jk[2].J"),
            CONNECT("k3Or.out0", "jk[3].K"),
            CONNECT("j3Or.out0", "jk[3].J"),
            // connections for LOAD data invertor
            CONNECT("j0Or.out0", "j0Not.in0"),
            CONNECT("j0Not.out0", "k0And1.in1"),
            CONNECT("j1Or.out0", "j1Not.in0"),
            CONNECT("j1Not.out0", "k1And1.in1"),
            CONNECT("j2Or.out0", "j2Not.in0"),
            CONNECT("j2Not.out0", "k2And1.in1"),
            CONNECT("j3Or.out0", "j3Not.in0"),
            CONNECT("j3Not.out0", "k3And1.in1"),
            // connections for Output
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("jk.Q", "Q", BITS)
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<SynchronousBinaryCounter>(name);
    }
    SynchronousBinaryCounter(const std::string& name)
        : ComponentBase(detail::getDescription<SynchronousBinaryCounter>(), name)
    {}
    virtual ~SynchronousBinaryCounter() {}
};

template <size_t BITS>
class ProgramCounter : public ComponentBase
{
public:
    static constexpr size_t CHIP_BITS = 4;
    static_assert((BITS % CHIP_BITS) == 0 && BITS >= CHIP_BITS, "BITS must be multiple of 4 and at least 4");
    static constexpr size_t CHIPS     = BITS / CHIP_BITS;
    static const char* Name()
    {
        static std::string name{"ProgramCounter" + std::to_string(BITS)};
        return name.c_str();
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN("CLK", ForwardInputPin),
            DEFINE_PIN("J", ForwardInputPin),     // load from bus
            DEFINE_PIN("CE", ForwardInputPin),    // counter enable
            DEFINE_PIN("CO", ForwardInputPin),    // counter out
            DEFINE_PIN("CLR", ForwardInputPin),
            DEFINE_PIN_ARRAY("D", ForwardInputPin, BITS),
            DEFINE_PIN_ARRAY("Q", ForwardOutputPin, BITS),
            DEFINE_PIN("RC", ForwardOutputPin)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT_ARRAY("sbc", SynchronousBinaryCounter, CHIPS),
            DEFINE_SUBCOMPONENT_ARRAY("rcAnd", ANDGateComponent, CHIPS),
            DEFINE_SUBCOMPONENT_ARRAY("en", BusBufferComponent, BITS)
        );
    }
    static constexpr auto Connections()
    {
        auto conn0 = std::make_tuple(
            CONNECT_MULTICAST_COMPONENT("CLK", "sbc.CLK", CHIPS),
            CONNECT("CE", "sbc[0].ENABLE"),
            CONNECT("CE", "rcAnd[0].in1"),
            CONNECT_MULTICAST_COMPONENT("J", "sbc.LOAD", CHIPS),
            CONNECT_MULTICAST_COMPONENT("CLR", "sbc.CLR", CHIPS),
            CONNECT_MULTICAST_COMPONENT("CO", "en.in1", BITS),
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("en.out0", "Q", BITS),
            CONNECT_COMPONENT_ARRAY_2_COMPONENT_ARRAY("sbc.RC", "rcAnd.in0", CHIPS),
            CONNECT(sp("rcAnd[", CHIPS - 1, "].out0"), "RC")
        );
        return std::tuple_cat(
            conn0,
            connectRC(std::make_index_sequence<CHIPS - 1>{}),
            connectD(std::make_index_sequence<CHIPS>{}),
            connectQ(std::make_index_sequence<CHIPS>{})
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<ProgramCounter>(name);
    }
    ProgramCounter(const std::string& name)
        : ComponentBase(detail::getDescription<ProgramCounter>(), name)
    {}
    virtual ~ProgramCounter() {}
private:
    template <size_t... I>
    static constexpr auto connectRC(const std::index_sequence<I...>&)
    {
        return std::make_tuple(
            CONNECT(sp("rcAnd[", I, "].out0"), sp("sbc[", I + 1, "].ENABLE"))...,
            CONNECT(sp("rcAnd[", I, "].out0"), sp("rcAnd[", I + 1, "].in1"))...
        );
    }
    template <size_t... C>
    static constexpr auto connectD(const std::index_sequence<C...>&)
    {
        return std::tuple_cat(connectChipD<C>(std::make_index_sequence<CHIP_BITS>{})...);
    }
    template <size_t... C>
    static constexpr auto connectQ(const std::index_sequence<C...>&)
    {
        return std::tuple_cat(connectChipQ<C>(std::make_index_sequence<CHIP_BITS>{})...);
    }
    template <size_t C, size_t... I>
    static constexpr auto connectChipD(const std::index_sequence<I...>&)
    {
        return std::make_tuple(CONNECT(sp("D[", C * CHIP_BITS + I, "]"), sp("sbc[", C, "].D[", I, "]"))...);
    }
    template <size_t C, size_t... I>
    static constexpr auto connectChipQ(const std::index_sequence<I...>&)
    {
        return std::make_tuple(CONNECT(sp("sbc[", C, "].Q[", I, "]"), sp("en[", C * CHIP_BITS + I, "].in0"))...);
    }
};

} // namespace component

#endif // PROGRAM_COUNTER_HPP__
