#ifndef GATE_TEMPLATE_HPP_
#define GATE_TEMPLATE_HPP_

#include <array>
#include <memory>
#include <type_traits>

#include <IGate.hpp>
#include <IPin.hpp>

namespace gate
{

class TypePackBase {};
template <typename... Ts>
class TypePack : TypePackBase {};

template <typename InputPinT,
         typename TruthTableT,
         typename OutputT>
class GateTemplate
{
    static_assert(std::is_base_of<TruthTableBase, TruthTableT>::value, "TruthTableT must be inherited from TruthTableBase");
    static_assert(std::is_base_of<TypePackBase, InputPinT>::value && std::is_base_of<TypePackBase, OutputT>::value,
            "You need to use TypePack to wrap the input and output pin type");
};

template <typename... InputPinT,
         typename TruthTableT,
         typename... OutputPinT>
class GateTemplate<TypePack<InputPinT...>,
      TruthTableT,
      TypePack<OutputPinT...>> : public IGate
{
public:
    static_assert(std::is_base_of<TruthTableBase, TruthTableT>::value, "TruthTableT must be inherited from TruthTableBase");
    GateTemplate(const std::string name = "")
       : m_inputPins{std::make_unique<InputPinT>(this)...}
       , m_outputPins{std::make_unique<OutputPinT>(this)...}
       , m_name{name}
    {
        for (size_t i = 0; i < sizeof...(InputPinT); ++i)
        {
            m_inputPins[i]->index(i);
        }
        for (size_t i = 0; i < sizeof...(OutputPinT); ++i)
        {
            m_outputPins[i]->index(i);
        }
        // using default input state compute right output state
        // following operation is performed before any IPin::connect()
        m_truthTable.compute(m_inputPins, m_outputPins);
    }
    virtual void compute() override
    {
        m_truthTable.compute(m_inputPins, m_outputPins);
    }
    virtual IPin* input(size_t index) override
    {
        return m_inputPins[index].get();
    }
    virtual ISourcePin* output(size_t index) override
    {
        return m_outputPins[index].get();
    }
    virtual const std::string& name() const override
    {
        return m_name;
    }
protected:
    static const TruthTableT m_truthTable;
    std::array<std::unique_ptr<IPin>, sizeof...(InputPinT)> m_inputPins;
    std::array<std::unique_ptr<ISourcePin>, sizeof...(OutputPinT)> m_outputPins;
    std::string m_name;
};

template <typename... InputPinT, typename TruthTableT, typename... OutputPinT>
const TruthTableT GateTemplate<TypePack<InputPinT...>, TruthTableT, TypePack<OutputPinT...>>::m_truthTable;

} // namespace gate

#endif
