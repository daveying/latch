#ifndef OR_GATE_HPP__
#define OR_GATE_HPP__

#include <ZeroDelayPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

// ID = 3, InputSize = 2, OutputSize = 1
static constexpr GateID ORGateID         = 3;
static constexpr size_t ORGateInputSize  = 2;
static constexpr size_t ORGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(ORGate, ORGateID, ORGateInputSize, ORGateOutputSize)
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
END_GATE_TRUTH_TABLE(ORGate, ORGateID, ORGateInputSize, ORGateOutputSize)

using ORGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    gate_info<ORGateID>::TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // OR_GATE_HPP__
