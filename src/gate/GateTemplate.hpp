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

class GateTemplateBase {};

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
      TypePack<OutputPinT...>> : public IGate, GateTemplateBase
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
    virtual IPin* pin(size_t index) override
    {
        if (index >= sizeof...(InputPinT))
        {
            return m_outputPins[index - sizeof...(InputPinT)].get();
        }
        return m_inputPins[index].get();
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
