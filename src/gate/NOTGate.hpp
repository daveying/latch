#ifndef NOT_GATE_HPP__
#define NOT_GATE_HPP__

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

static constexpr size_t NOTGateInputSize  = 1;
static constexpr size_t NOTGateOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(NOTGate, NOTGateInputSize, NOTGateOutputSize)
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
END_GATE_TRUTH_TABLE

using NOTGate = GateTemplate<
    TypePack<ZeroDelayInputPin>,
    NOTGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

using NOTGateDelayed = GateTemplate<
    TypePack<DelayedInputPin<1>>,
    NOTGate_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // NOR_GATE_HPP__
