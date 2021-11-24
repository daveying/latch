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

#ifndef REGISTER_HPP__
#define REGISTER_HPP__

#include <IComponent.hpp>
#include <ComponentBase.hpp>
#include <ComponentDescription.hpp>
#include <GateComponents.hpp>

namespace component
{

class DFlipFlop : public ComponentBase
{
public:
    static constexpr const char* Name = "DFlipFlop";
    static constexpr auto pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN("D", ForwardInputPin),
            DEFINE_PIN("Q", ForwardOutputPin),
            DEFINE_PIN("Qc", ForwardOutputPin)
        );
    }
    static constexpr auto subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("invertor", NOTGateDelayedComponent),
            DEFINE_SUBCOMPONENT("pulseGate", ANDGateComponent),
            DEFINE_SUBCOMPONENT("andGate0", ANDGateComponent),
            DEFINE_SUBCOMPONENT("andGate1", ANDGateComponent),
            DEFINE_SUBCOMPONENT("notGate", NOTGateComponent),
            DEFINE_SUBCOMPONENT("norGate0", NORGateComponent),
            DEFINE_SUBCOMPONENT("norGate1", NORGateComponent)
        );
    }
    static constexpr auto connections()
    {
        return std::make_tuple(
            CONNECT("D", "andGate0.in0"),
            CONNECT("D", "notGate.in0"),
            CONNECT("Clock", "invertor.in0"),
            CONNECT("Clock", "pulseGate.in0"),
            CONNECT("invertor.out0", "pulseGate.in1"),
            CONNECT("pulseGate.out0", "andGate0.in1"),
            CONNECT("pulseGate.out0", "andGate1.in0"),
            CONNECT("notGate.out0", "andGate1.in1"),
            CONNECT("andGate0.out0", "norGate0.in0"),
            CONNECT("andGate1.out0", "norGate1.in1"),
            CONNECT("norGate1.out0", "norGate0.in1"),
            CONNECT("norGate0.out0", "norGate1.in0"),
            CONNECT("norGate0.out0", "Qc"),
            CONNECT("norGate1.out0", "Q")
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<DFlipFlop>(name);
    }
    DFlipFlop(const std::string& name)
        : ComponentBase(detail::getDescription<DFlipFlop>(), name)
    {}
    virtual ~DFlipFlop() {}
};

class Bit : public ComponentBase
{
public:
    static constexpr const char* Name = "Bit";
    static constexpr auto pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Load", ForwardInputPin),
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN("D", ForwardInputPin),
            DEFINE_PIN("Q", ForwardOutputPin)
        );
    }
    static constexpr auto subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT("dataInvertor", NOTGateComponent),
            DEFINE_SUBCOMPONENT("feedbackAnd", ANDGateComponent),
            DEFINE_SUBCOMPONENT("dataAnd", ANDGateComponent),
            DEFINE_SUBCOMPONENT("dataSelector", ORGateComponent),
            DEFINE_SUBCOMPONENT("dff", DFlipFlop)
        );
    }
    static constexpr auto connections()
    {
        return std::make_tuple(
            CONNECT("Load", "dataInvertor.in0"),
            CONNECT("Load", "dataAnd.in0"),
            CONNECT("D", "dataAnd.in1"),
            CONNECT("Clock", "dff.Clock"),
            CONNECT("dataInvertor.out0", "feedbackAnd.in1"),
            CONNECT("feedbackAnd.out0", "dataSelector.in0"),
            CONNECT("dataAnd.out0", "dataSelector.in1"),
            CONNECT("dataSelector.out0", "dff.D"),
            CONNECT("dff.Q", "feedbackAnd.in0"),
            CONNECT("dff.Q", "Q")
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<Bit>(name);
    }
    Bit(const std::string& name)
        : ComponentBase(detail::getDescription<Bit>(), name)
    {}
    virtual ~Bit() {}
};

template <size_t BITS>
class Register : public ComponentBase
{
public:
    static constexpr const char* Name = "Register";
    static constexpr auto pins()
    {
        return std::make_tuple(
            DEFINE_PIN("Load", ForwardInputPin),
            DEFINE_PIN("Clock", ForwardInputPin),
            DEFINE_PIN_ARRAY("D", ForwardInputPin, BITS),
            DEFINE_PIN_ARRAY("Q", ForwardOutputPin, BITS)
        );
    }
    static constexpr auto subcomponents()
    {
        return std::make_tuple(
            DEFINE_SUBCOMPONENT_ARRAY("bit", Bit, BITS)
        );
    }
    static constexpr auto connections()
    {
        return std::make_tuple(
            CONNECT_MULTICAST("Load", "bit.Load", BITS),
            CONNECT_MULTICAST("Clock", "bit.Clock", BITS),
            CONNECT_ARRAY("D", "bit.D", BITS),
            CONNECT_ARRAY("Q", "bit.Q", BITS)
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<Register>(name);
    }
    Register(const std::string& name)
        : ComponentBase(detail::getDescription<Register>(), name)
    {}
    virtual ~Register() {}
};

} // namespace component

#endif // REGISTER_HPP__
