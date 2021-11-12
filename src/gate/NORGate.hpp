#ifndef NOR_GATE_HPP__
#define NOR_GATE_HPP__

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
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

} // namespace gate

#endif // NOR_GATE_HPP__
