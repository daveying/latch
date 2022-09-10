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

#ifndef GATE_COMPONENTS_HPP__
#define GATE_COMPONENTS_HPP__

#include <ANDGate.hpp>
#include <BusBuffer.hpp>
#include <NANDGate.hpp>
#include <NORGate.hpp>
#include <NOTGate.hpp>
#include <ORGate.hpp>
#include <XORGate.hpp>
#include <IComponent.hpp>
#include <ComponentDescription.hpp>

namespace component
{

template <typename T>
struct GateComponentName
{
};

template <typename T>
class GateComponentTemplate
{
    static_assert(std::is_base_of<GateTemplateBase, T>::value, "T must be an instantiation of GateTemplate");
};

template <typename... InputPinT,
         typename TruthTableT,
         typename... OutputPinT>
class GateComponentTemplate<
    GateTemplate<TypePack<InputPinT...>,
                 TruthTableT,
                 TypePack<OutputPinT...>>> : public IComponent
{
public:
    using GateT = GateTemplate<TypePack<InputPinT...>, TruthTableT, TypePack<OutputPinT...>>;
    GateComponentTemplate(const std::string& name)
        : m_gate{name}
    {}
    static inline constexpr size_t inputSize()
    {
        return sizeof...(InputPinT);
    }
    static inline constexpr size_t outputSize()
    {
        return sizeof...(OutputPinT);
    }
    static constexpr const char* Name()
    {
        return GateComponentName<GateT>::Name();
    }
    static constexpr auto Pins()
    {
        return detail::gatePins<InputPinT..., OutputPinT...>(sizeof...(InputPinT));
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple();
    }
    static constexpr auto Connections()
    {
        return std::make_tuple();
    }
    virtual void initialize() override
    {
        m_gate.initialize();
    }
    virtual const std::string& name() const override
    {
        return m_gate.name();
    }
    virtual IPin* pin(size_t idx) override
    {
        IPin* ret{nullptr};
        if (idx >= inputSize() + outputSize())
        {
            throw std::runtime_error("Invalid pin index: " + std::to_string(idx) + ", for component " + m_gate.name());
        }
        else if (idx >= inputSize())
        {
            ret = m_gate.output(idx - inputSize());
        }
        else
        {
            ret = m_gate.input(idx);
        }
        return ret;
    }
    virtual IComponent* subcomponent(int) override
    {
        return nullptr;
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<GateComponentTemplate>(name);
    }
protected:
    GateT m_gate;
};

#define DEFINE_GATE_COMPONENT(NAME, GATE)       \
    using NAME = GateComponentTemplate<GATE>;   \
    template <>                                 \
    struct GateComponentName<GATE>              \
    {                                           \
        static constexpr const char* Name()     \
        {                                       \
            return #NAME;                       \
        }                                       \
    };

DEFINE_GATE_COMPONENT(ANDGateComponent, ANDGate);
DEFINE_GATE_COMPONENT(ANDGate3Component, ANDGate3);
DEFINE_GATE_COMPONENT(ANDGate4Component, ANDGate4);
DEFINE_GATE_COMPONENT(ANDGate5Component, ANDGate5);
DEFINE_GATE_COMPONENT(BusBufferComponent, BusBuffer);
DEFINE_GATE_COMPONENT(BusBufferDataDelayedComponent, BusBufferDataDelayed);
DEFINE_GATE_COMPONENT(NANDGateComponent, NANDGate);
DEFINE_GATE_COMPONENT(NORGateComponent, NORGate);
DEFINE_GATE_COMPONENT(NORGate3Component, NORGate3);
DEFINE_GATE_COMPONENT(NORGate4Component, NORGate4);
DEFINE_GATE_COMPONENT(NORGateUnitInertial4Component, NORGateUnitInertial4);
DEFINE_GATE_COMPONENT(NORGateDelayedComponent, NORGateDelayed);
DEFINE_GATE_COMPONENT(NOTGateComponent, NOTGate);
DEFINE_GATE_COMPONENT(NOTGateUnitInertialComponent, NOTGateUnitInertial);
DEFINE_GATE_COMPONENT(NOTGateDelayedComponent, NOTGateDelayed);
DEFINE_GATE_COMPONENT(ORGateComponent, ORGate);
DEFINE_GATE_COMPONENT(XORGateComponent, XORGate);

} // namespace component

#endif // GATE_COMPONENTS_HPP__
