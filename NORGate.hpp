#ifndef NOR_GATE_HPP__
#define NOR_GATE_HPP__

#include <ZeroDelayPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

// ID = 1, InputSize = 2, OutputSize = 1
static constexpr size_t NORGateID         = 1;
static constexpr size_t NORGateInputSize  = 2;
static constexpr size_t NORGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NORGate, NORGateID, NORGateInputSize, NORGateOutputSize)
{
    if (input[0]->value() != input[1]->value())
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
    TypePack<ZeroDelayPin, ZeroDelayPin>,
    gate_info<NORGateID>::TruthTable,
    TypePack<ZeroDelayPin>>;

} // namespace gate

#endif
