#ifndef NAND_GATE_HPP_
#define NAND_GATE_HPP_

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

// ID = 2, InputSize = 2, OutputSize = 1
static constexpr GateID NANDGateID         = 2;
static constexpr size_t NANDGateInputSize  = 2;
static constexpr size_t NANDGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NANDGate, NANDGateID, NANDGateInputSize, NANDGateOutputSize)
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
END_GATE_TRUTH_TABLE(NANDGate, NANDGateID, NANDGateInputSize, NANDGateOutputSize)

using NANDGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    gate_info<NANDGateID>::TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // NAND_GATE_HPP_
