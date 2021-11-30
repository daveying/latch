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

#ifndef NOR_GATE_HPP__
#define NOR_GATE_HPP__

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace component
{

static constexpr size_t NORGateInputSize  = 2;
static constexpr size_t NORGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NORGate, NORGateInputSize, NORGateOutputSize)
{
    if (input[0]->value() == PinState::Low && input[1]->value() == PinState::Low)
    {
        output[0]->value(PinState::High);
    }
    else
    {
        output[0]->value(PinState::Low);
    }
}
END_GATE_TRUTH_TABLE

using NORGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    NORGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

using NORGateDelayed = GateTemplate<
    TypePack<DelayedInputPin<1>, ZeroDelayInputPin>,
    NORGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace component

#endif // NOR_GATE_HPP__
