#ifndef NOR_GATE_HPP__
#define NOR_GATE_HPP__

#include <ZeroDelayPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

// ID = 1, InputSize = 2, OutputSize = 1
DEFINE_GATE_TRUTH_TABLE(NORGate, 1, 2, 1)
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
END_GATE_TRUTH_TABLE

using NORGate = GateTemplate<
    TypePack<ZeroDelayPin, ZeroDelayPin>,
    gate_info<1>::TruthTable,
    TypePack<ZeroDelayPin>>;

} // namespace gate

#endif
