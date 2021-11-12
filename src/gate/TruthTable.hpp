#ifndef TRUTH_TABLE_HPP_
#define TRUTH_TABLE_HPP_

#include <array>
#include <memory>
#include <type_traits>

#include <IGate.hpp>
#include <IPin.hpp>

namespace gate
{

#define DEFINE_GATE_TRUTH_TABLE(_Name, _InputSize, _OutputSize)                   \
    class _Name##_TruthTable : public TruthTableBase                              \
    {                                                                             \
    public:                                                                       \
        using InputArray = std::array<std::unique_ptr<IPin>, _InputSize>;         \
        using OutputArray = std::array<std::unique_ptr<ISourcePin>, _OutputSize>; \
        void compute(const InputArray& input, const OutputArray& output) const
#define END_GATE_TRUTH_TABLE };

} // namespace gate

#endif
