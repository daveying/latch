#ifndef BUS_BUFFER_HPP__
#define BUS_BUFFER_HPP__

#include <DelayedPin.hpp>
#include <GateTemplate.hpp>
#include <TruthTable.hpp>

namespace gate
{

static constexpr size_t BusBufferInputSize  = 2;
static constexpr size_t BusBufferOutputSize = 1;
DEFINE_GATE_TRUTH_TABLE(BusBuffer, BusBufferInputSize, BusBufferOutputSize)
{
    // only when control signal is high, will forward the exact
    // value of input to output
    if (input[1]->value() == PinState::High)
    {
        output[0]->value(input[0]->value());
    }
}
END_GATE_TRUTH_TABLE

using BusBuffer = GateTemplate<
    TypePack<ZeroDelayInputPin, ZeroDelayInputPin>,
    BusBuffer_TruthTable,
    TypePack<ZeroDelayOutputPin>>;

} // namespace gate

#endif // BUS_BUFFER_HPP__
