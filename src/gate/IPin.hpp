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

#ifndef IPIN_HPP_
#define IPIN_HPP_

#include <cstdint>
#include <vector>
#include <string>

namespace component
{
class IComponent;

using PinStateType = uint8_t;

enum class PinState : PinStateType
{
    Low  = 0,
    High = 1
};

inline PinState operator~(PinState value)
{
    PinState ret;
    if (value == PinState::Low)
    {
        ret = PinState::High;
    }
    else
    {
        ret = PinState::Low;
    }
    return ret;
}

inline std::string pinStateStr(const PinState& state)
{
    std::string ret;
    if (state == PinState::Low)
    {
        ret = "Low";
    }
    else
    {
        ret = "High";
    }
    return ret;
}

class IPin
{
public:
    virtual const component::IComponent* parent() const = 0;
    virtual component::IComponent* parent()             = 0;
    virtual PinState value() const                      = 0;
    virtual void value(PinState newVal)                 = 0;
    virtual int64_t index() const                       = 0;
    virtual void index(int64_t)                         = 0;
};

class ISourcePin : public IPin
{
public:
    virtual void connect(IPin* peer)    = 0;
    virtual std::vector<IPin*>& peers() = 0;
};

} // namespace component

#endif
