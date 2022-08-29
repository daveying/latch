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

#ifndef IGATE_HPP__
#define IGATE_HPP__

#include <cstddef>
#include <string>
#include <IPin.hpp>

namespace component
{

enum class PinDirection
{
    INPUT,
    OUTPUT,
};

class IComponent
{
public:
    virtual void initialize()                 = 0;
    virtual IComponent* subcomponent(int idx) = 0;
    virtual IPin* pin(size_t idx)       = 0;
    virtual const std::string& name() const   = 0;
};

class IGate : public IComponent
{
public:
    virtual void compute()                   = 0;
    virtual IPin* input(size_t index)        = 0;
    virtual ISourcePin* output(size_t index) = 0;
    virtual void initialize() final
    {
        compute();
    }
    virtual component::IComponent* subcomponent(int) final
    {
        return nullptr;
    };
};

class TruthTableBase {};
} // namespace component

namespace std
{

static inline std::string to_string(component::PinDirection direction)
{
    std::string ret;
    if (direction == component::PinDirection::INPUT)
    {
        ret = "INPUT";
    }
    else
    {
        ret = "OUTPUT";
    }
    return ret;
}

} // namespace std
#endif
