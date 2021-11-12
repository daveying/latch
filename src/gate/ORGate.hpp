#ifndef OR_GATE_HPP__
#define OR_GATE_HPP__

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

static constexpr size_t ORGateInputSize  = 2;
static constexpr size_t ORGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(ORGate, ORGateInputSize, ORGateOutputSize)
{
    if (input[0]->value() == PinState::Low && input[1]->value() == PinState::Low)
    {
        output[0]->value(PinState::Low);
    }
    else
    {
        output[0]->value(PinState::High);
    }
}
END_GATE_TRUTH_TABLE

using ORGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    ORGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // OR_GATE_HPP__
