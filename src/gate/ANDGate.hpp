#ifndef AND_GATE_HPP_
#define AND_GATE_HPP_

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
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

using ANDGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    ANDGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // AND_GATE_HPP_
