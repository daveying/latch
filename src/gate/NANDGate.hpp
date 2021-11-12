#ifndef NAND_GATE_HPP_
#define NAND_GATE_HPP_

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

static constexpr size_t NANDGateInputSize  = 2;
static constexpr size_t NANDGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NANDGate, NANDGateInputSize, NANDGateOutputSize)
{
    if (input[0]->value() == PinState::High && input[1]->value() == PinState::High)
    {
        output[0]->value(PinState::Low);
    }
    else
    {
        output[0]->value(PinState::High);
    }
}
END_GATE_TRUTH_TABLE

using NANDGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    NANDGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // NAND_GATE_HPP_
