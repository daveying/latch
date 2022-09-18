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

#ifndef ALU_HPP__
#define ALU_HPP__

#include <IComponent.hpp>
#include <ComponentBase.hpp>
#include <ComponentDescription.hpp>
#include <GateComponents.hpp>

namespace component
{

class BitAdder : public ComponentBase
{
public:
    static constexpr const char* Name()
    {
        return "BitAdder";
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN("A", ForwardInputPin),
            DEFINE_PIN("B", ForwardInputPin),
            DEFINE_PIN("Ci", ForwardInputPin),
            DEFINE_PIN("Sum", ForwardOutputPin),
            DEFINE_PIN("Co", ForwardOutputPin)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("xor0", XORGateComponent),
            DEFINE_SUBCOMPONENT("xor1", XORGateComponent),
            DEFINE_SUBCOMPONENT("and0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("and1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("or", ORGateComponent)
        );
    }
    static constexpr auto Connections()
    {
        return std::make_tuple(
            CONNECT("A", "xor0.in0"),
            CONNECT("A", "and0.in1"),
            CONNECT("B", "xor0.in1"),
            CONNECT("B", "and0.in0"),
            CONNECT("Ci", "xor1.in1"),
            CONNECT("Ci", "and1.in0"),
            CONNECT("xor0.out0", "xor1.in0"),
            CONNECT("xor0.out0", "and1.in1"),
            CONNECT("and0.out0", "or.in1"),
            CONNECT("and1.out0", "or.in0"),
            CONNECT("xor1.out0", "Sum"),
            CONNECT("or.out0", "Co")
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<BitAdder>(name);
    }
    BitAdder(const std::string& name)
        : ComponentBase(detail::getDescription<BitAdder>(), name)
    {}
    virtual ~BitAdder() {}
};

template <size_t BITS>
class BinaryAdder : public ComponentBase
{
public:
    static_assert(BITS >= 1, "BITS must >= 1");
    static const char* Name()
    {
        static std::string name{"BinaryAdder" + std::to_string(BITS)};
        return name.c_str();
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN_ARRAY("A", ForwardInputPin, BITS),
            DEFINE_PIN_ARRAY("B", ForwardInputPin, BITS),
            DEFINE_PIN("Ci", ForwardInputPin),
            DEFINE_PIN_ARRAY("Sum", ForwardOutputPin, BITS),
            DEFINE_PIN("Co", ForwardOutputPin)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT_ARRAY("adder", BitAdder, BITS)
        );
    }
    static constexpr auto Connections()
    {
        auto conn0 = std::make_tuple(
            CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY("A", "adder.A", BITS),
            CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY("B", "adder.B", BITS),
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("adder.Sum", "Sum", BITS),
            CONNECT("Ci", "adder[0].Ci"),
            CONNECT(sp("adder[", BITS - 1, "].Co"), "Co")
        );
        auto conn1 = connectCarryPins(std::make_index_sequence<BITS - 1>{});
        return detail::cmb(conn0, conn1);
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<BinaryAdder>(name);
    }
    BinaryAdder(const std::string& name)
        : ComponentBase(detail::getDescription<BinaryAdder>(), name)
    {}
    virtual ~BinaryAdder() {}
private:
    template <size_t... I>
    static constexpr auto connectCarryPins(const std::index_sequence<I...>&)
    {
        return std::make_tuple(
            CONNECT(sp("adder[", I, "].Co"), sp("adder[", I + 1, "].Ci"))...
        );
    }
};

template <size_t BITS>
class ALU : public ComponentBase
{
public:
    static_assert(BITS >=1, "BITS must >= 1");
    static const char* Name()
    {
        static std::string name{"ALU" + std::to_string(BITS)};
        return name.c_str();
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN_ARRAY("A", ForwardInputPin, BITS),
            DEFINE_PIN_ARRAY("B", ForwardInputPin, BITS),
            DEFINE_PIN("EO", ForwardInputPin),
            DEFINE_PIN("SUB", ForwardInputPin),
            DEFINE_PIN_ARRAY("Sum", ForwardOutputPin, BITS),
            DEFINE_PIN("Co", ForwardOutputPin)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT_ARRAY("adder", BitAdder, BITS),
            DEFINE_SUBCOMPONENT_ARRAY("negater", XORGateComponent, BITS),
            DEFINE_SUBCOMPONENT_ARRAY("enable", BusBufferDataDelayedComponent, BITS)
        );
    }
    static constexpr auto Connections()
    {
        auto conn0 = std::make_tuple(
            CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY("A", "adder.A", BITS),
            CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY("B", "negater.in0", BITS),
            CONNECT_COMPONENT_ARRAY_2_COMPONENT_ARRAY("negater.out0", "adder.B", BITS),
            CONNECT_COMPONENT_ARRAY_2_COMPONENT_ARRAY("adder.Sum", "enable.in0", BITS),
            CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY("enable.out0", "Sum", BITS),
            CONNECT_MULTICAST_COMPONENT("EO", "enable.in1", BITS),
            CONNECT_MULTICAST_COMPONENT("SUB", "negater.in1", BITS),
            CONNECT("SUB", "adder[0].Ci"),
            CONNECT(sp("adder[", BITS - 1, "].Co"), "Co")
        );
        auto conn1 = connectCarryPins(std::make_index_sequence<BITS - 1>{});
        return detail::cmb(conn0, conn1);
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<ALU>(name);
    }
    ALU(const std::string& name)
        : ComponentBase(detail::getDescription<ALU>(), name)
    {}
    virtual ~ALU() {}
private:
    template <size_t... I>
    static constexpr auto connectCarryPins(std::index_sequence<I...>)
    {
        return std::make_tuple(
            CONNECT(sp("adder[", I, "].Co"), sp("adder[", I + 1, "].Ci"))...
        );
    }
};

} // namespace component

#endif // ALU_HPP__
