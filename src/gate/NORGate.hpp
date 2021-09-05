#ifndef NOR_GATE_HPP__
#define NOR_GATE_HPP__

#include <ZeroDelayPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

// ID = 1, InputSize = 2, OutputSize = 1
static constexpr GateID NORGateID         = 1;
static constexpr size_t NORGateInputSize  = 2;
static constexpr size_t NORGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NORGate, NORGateID, NORGateInputSize, NORGateOutputSize)
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
END_GATE_TRUTH_TABLE(NORGate, NORGateID, NORGateInputSize, NORGateOutputSize)

using NORGate = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    gate_info<NORGateID>::TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // NOR_GATE_HPP__
