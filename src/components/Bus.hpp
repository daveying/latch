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

#ifndef BUS_HPP__
#define BUS_HPP__

#include <ComponentDescription.hpp>
#include <ComponentBase.hpp>

namespace component
{

template <size_t BITS>
class Bus : public ComponentBase
{
public:
    static_assert(BITS >= 1, "BITS must >= 1");
    static const char* Name()
    {
        static std::string name{"Bus" + std::to_string(BITS)};
        return name.c_str();
    }
    static constexpr auto Pins()
    {
        return std::make_tuple(
            DEFINE_PIN_ARRAY("I", ForwardInputPin, BITS),
            DEFINE_PIN_ARRAY("O", ForwardOutputPin, BITS)
        );
    }
    static constexpr auto Subcomponents()
    {
        return std::make_tuple();
    }
    static constexpr auto Connections()
    {
        return std::make_tuple(
            CONNECT_PIN_ARRAY_2_PIN_ARRAY("I", "O", BITS)
        );
    }
    static std::unique_ptr<IComponent> create(const std::string& name)
    {
        return std::make_unique<Bus>(name);
    }
    Bus(const std::string& name)
        : ComponentBase(detail::getDescription<Bus>(), name)
    {}
    virtual ~Bus() {}
};

} // namespace component

#endif // BUS_HPP__
