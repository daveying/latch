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

} // namespace component

#endif // PROGRAM_COUNTER_HPP__
