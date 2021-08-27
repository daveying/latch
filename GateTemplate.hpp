#ifndef GATE_TEMPLATE_HPP_
#define GATE_TEMPLATE_HPP_

#include <array>

#include <IGate.hpp>
namespace gate
{

template <typename InputPinTuple,
         typename TruthTableT,
         typename OutputTuple>
class GateTemplate
{
    static_assert("You need to use TypePack to wrap the input and output pin type");
};

template <typename... Ts>
class TypePack {};
template <typename... InputPinT,
         typename TruthTableT,
         typename... OutputPinT>
class GateTemplate <TypePack<InputPinT...>,
      TruthTableT,
      TypePack<OutputPinT...>> : public IGate
{
public:
    GateTemplate()
       : m_inputPins{std::make_unique<InputPinT>(this)...}
       , m_outputPins{std::make_unique<OutputPinT>(this)...}
    virtual void compute() override
    {
        compute(m_inputPins, m_outputPins);
    }
protected:
    static const TruthTableT m_truthTable;
    std::array<std::unique_ptr<IPin>, sizeof...(InputPinT)> m_inputPins;
    std::array<std::unique_ptr<IPin>, sizeof...(OutputPinT)> m_outputPins;
};

} // namespace gate

#endif
