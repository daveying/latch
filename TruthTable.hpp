#ifndef TRUTH_TABLE_HPP_
#define TRUTH_TABLE_HPP_

#include <array>
#include <memory>

#include <IGate.hpp>
#include <IPin.hpp>

namespace gate
{

class TruthTableBase {};

template <GateID ID, size_t InputSize, size_t OutputSize>
class _TruthTable : public TruthTableBase
{
    static_assert("Gate ID invalid");
};

#define DEFINE_GATE_TRUTH_TABLE(_Name, _ID, _InputSize, _OutputSize)        \
    template <>                                                             \
    class _TruthTable<_ID, _InputSize, _OutputSize>                         \
    {                                                                       \
    public:                                                                 \
        static constexpr GateID ID = _ID;                                   \
        using InputArray = std::array<std::unique_ptr<IPin>, _InputSize>;   \
        using OutputArray = std::array<std::unique_ptr<IPin>, _OutputSize>; \
        void compute(const InputArray& input, const OutputArray& output) const

#define END_GATE_TRUTH_TABLE(_Name, _ID, _InputSize, _OutputSize)           \
    };                                                                      \
    template <>                                                             \
    class gate_info<_ID>                                                    \
    {                                                                       \
    public:                                                                 \
        static constexpr size_t InputSize  = _InputSize;                    \
        static constexpr size_t OutputSize = _OutputSize;                   \
        static constexpr const char* Name  = #_Name;                        \
        using TruthTable = _TruthTable<_ID, _InputSize, _OutputSize>;       \
    };                                                                      \

} // namespace gate

#endif
