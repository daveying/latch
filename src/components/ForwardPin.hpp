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

#ifndef FORWARD_PIN_HPP__ 
#define FORWARD_PIN_HPP__ 

#include <IPin.hpp>
#include <IComponent.hpp>
#include <ComponentDescription.hpp>

namespace component
{

template <PinDirection Direction>
class ForwardPin : public gate::ISourcePin
{
public:
    static constexpr auto direction()
    {
        return Direction;
    }

    explicit ForwardPin(IComponent* parent, int64_t index = -1)
        : m_parent{parent}
        , m_value{gate::PinState::Low}
        , m_index{index}
    {
    }

    virtual const IComponent* parent() const override
    {
        return m_parent;
    }
    virtual IComponent* parent() override
    {
        return m_parent;
    }

    virtual gate::PinState value() const override
    {
        return m_value;
    }

    // compute the name every time to save memory
    // revisit to determine if name should be stored
    // in a member
    std::string name() const
    {
        auto n = (m_parent ? m_parent->name() : "None")
            + ".fwd" + (direction() == PinDirection::INPUT ? "in" : "out")
            + std::to_string(m_index);
        return n;
    }

    virtual void value(gate::PinState newVal) override
    {
        m_value = newVal;
        for (auto peer : m_peers)
        {
            peer->value(newVal);
        }
    }

    virtual int64_t index() const override
    {
        return m_index;
    }

    virtual void index(int64_t idx) override
    {
        m_index = idx;
    }

    virtual void connect(IPin* peer) override
    {
        m_peers.push_back(peer);
    }

    virtual std::vector<IPin*>& peers() override
    {
        return m_peers;
    }

protected:
    IComponent* m_parent;
    gate::PinState m_value;
    std::vector<IPin*> m_peers;
    int64_t m_index;
};

using ForwardInputPin  = ForwardPin<PinDirection::INPUT>;
using ForwardOutputPin = ForwardPin<PinDirection::OUTPUT>;

} // namespace component

#endif // FORWARD_PIN_HPP__ 
