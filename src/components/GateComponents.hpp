#ifndef GATE_COMPONENTS_HPP__
#define GATE_COMPONENTS_HPP__

#include <ANDGate.hpp>
#include <BusBuffer.hpp>
#include <NANDGate.hpp>
#include <NORGate.hpp>
#include <NOTGate.hpp>
#include <ORGate.hpp>
#include <IComponent.hpp>

namespace component
{
using gate::TruthTableBase;
using gate::TypePackBase;
using gate::GateTemplateBase;
using gate::TypePack;

template <typename T>
class GateComponentTemplate
{
    static_assert(std::is_base_of<GateTemplateBase, T>::value, "T must be an instantiation of GateTemplate");
};

template <typename... InputPinT,
         typename TruthTableT,
         typename... OutputPinT>
class GateComponentTemplate<
    gate::GateTemplate<TypePack<InputPinT...>,
                       TruthTableT,
                       TypePack<OutputPinT...>>> : public IComponent
{
public:
    using GateT = gate::GateTemplate<TypePack<InputPinT...>, TruthTableT, TypePack<OutputPinT...>>;
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
    virtual void initialize() override
    {
        m_gate.compute();
    }
    virtual const std::string& name() const override
    {
        return m_gate.name();
    }
    virtual gate::IPin* pin(size_t idx) override
    {
        gate::IPin* ret{nullptr};
        if (idx >= inputSize() + outputSize())
        {
            throw std::runtime_error("Invalid pin index: " + std::to_string(idx) + ", for component " + m_gate.name());
        }
        else if (idx >= inputSize())
        {
            ret = m_gate.output(idx = inputSize());
        }
        else
        {
            ret = m_gate.input(idx);
        }
        return ret;
    }
    virtual IComponent* subcomponent(int) override
    {
        return &m_gate;
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<GateComponentTemplate>(name);
    }
protected:
    GateT m_gate;
};

using ANDGateComponent        = GateComponentTemplate<gate::ANDGate>;
using BusBufferComponent      = GateComponentTemplate<gate::BusBuffer>;
using NANDGateComponent       = GateComponentTemplate<gate::NANDGate>;
using NORGateComponent        = GateComponentTemplate<gate::NORGate>;
using NOTGateComponent        = GateComponentTemplate<gate::NOTGate>;
using NOTGateDelayedComponent = GateComponentTemplate<gate::NOTGateDelayed>;
using ORGateComponent         = GateComponentTemplate<gate::ORGate>;

} // namespace component

#endif // GATE_COMPONENTS_HPP__
