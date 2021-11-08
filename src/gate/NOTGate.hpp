#ifndef NOT_GATE_HPP__
#define NOT_GATE_HPP__

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

// ID = 5, InputSize = 1, OutputSize = 1
static constexpr GateID NOTGateID         = 5;
static constexpr size_t NOTGateInputSize  = 1;
static constexpr size_t NOTGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NOTGate, NOTGateID, NOTGateInputSize, NOTGateOutputSize)
{
    if (input[0]->value() == PinState::Low)
    {
        output[0]->value(PinState::High);
    }
    else
    {
        output[0]->value(PinState::Low);
    }
}
END_GATE_TRUTH_TABLE(NOTGate, NOTGateID, NOTGateInputSize, NOTGateOutputSize)

using NOTGate = GateTemplate<
    TypePack<ZeroDelayInputPin>,
    gate_info<NOTGateID>::TruthTable,
    TypePack<ZeroDelayOutputPin>>;

// TODO fix gate_info
using NOTGateDelayed = GateTemplate<
    TypePack<DelayedInputPin<1>>,
    gate_info<NOTGateID>::TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // NOR_GATE_HPP__
