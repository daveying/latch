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

#ifndef AND_GATE_HPP_
#define AND_GATE_HPP_

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace component
{

static constexpr size_t ANDGateInputSize  = 2;
static constexpr size_t ANDGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(ANDGate, ANDGateInputSize, ANDGateOutputSize)
{
    if (input[0]->value() == PinState::High && input[1]->value() == PinState::High)
    {
        output[0]->value(PinState::High);
    }
    else
    {
        output[0]->value(PinState::Low);
    }
}
END_GATE_TRUTH_TABLE

static constexpr size_t ANDGate3InputSize  = 3;
static constexpr size_t ANDGate3OutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(ANDGate3, ANDGate3InputSize, ANDGate3OutputSize)
{
    if (input[0]->value() == PinState::High
            && input[1]->value() == PinState::High
            && input[2]->value() == PinState::High)
    {
        output[0]->value(PinState::High);
    }
    else
    {
        output[0]->value(PinState::Low);
    }
}
END_GATE_TRUTH_TABLE

static constexpr size_t ANDGate4InputSize  = 4;
static constexpr size_t ANDGate4OutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(ANDGate4, ANDGate4InputSize, ANDGate4OutputSize)
{
    if (input[0]->value() == PinState::High
            && input[1]->value() == PinState::High
            && input[2]->value() == PinState::High
            && input[3]->value() == PinState::High)
    {
        output[0]->value(PinState::High);
    }
    else
    {
        output[0]->value(PinState::Low);
    }
}
END_GATE_TRUTH_TABLE

static constexpr size_t ANDGate5InputSize  = 5;
static constexpr size_t ANDGate5OutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(ANDGate5, ANDGate5InputSize, ANDGate5OutputSize)
{
    if (input[0]->value() == PinState::High
            && input[1]->value() == PinState::High
            && input[2]->value() == PinState::High
            && input[3]->value() == PinState::High
            && input[4]->value() == PinState::High)
    {
        output[0]->value(PinState::High);
    }
    else
    {
        output[0]->value(PinState::Low);
    }
}
END_GATE_TRUTH_TABLE

using ANDGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    ANDGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

using ANDGate3 = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin, ZeroDelayInputPin>,
    ANDGate3_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

using ANDGate4 = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin, ZeroDelayInputPin, ZeroDelayInputPin>,
    ANDGate4_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

using ANDGate5 = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin, ZeroDelayInputPin, ZeroDelayInputPin, ZeroDelayInputPin>,
    ANDGate5_TruthTable,
    TypePack<ZeroDelayOutputPin>>;
} // namespace component

#endif // AND_GATE_HPP_
