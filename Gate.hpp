#ifndef GATE_HPP__
#define GATE_HPP__

namespace gate
{

using GateID = size_t;

class IGate
{
public:
    virtual void compute() = 0;
};

enum class PinState : uint8_t
{
    Low  = 0,
    High = 1
};

class IPin
{
public:
    virtual const IGate* parent() = 0;
    virtual bool value()          = 0;
};

template <GateID ID>
class gate_info
{
    static_assert("Gate ID invalid");
};

class TruthTableBase {};

template <GateID ID, size_t InputSize, size_t OutputSize>
class TruthTable : public TruthTableBase
{
    static_assert("Gate ID invalid");
};

#define DEFINE_GATE_TRUTH_TABLE(_Name, _ID, _InputSize, _OutputSize)    \
    template <>                                                         \
    class gate_info<_ID>                                                \
    {                                                                   \
    public:                                                             \
        static constexpr size_t InputSize  = _InputSize;                \
        static constexpr size_t OutputSize = _OutputSize;               \
        static constexpr char* Name        = #_Name;                    \
    }                                                                   \
    template <>                                                         \
    class TruthTable<_ID, _InputSize, _OutputSize>

template <typename InputPinTuple,
         typename TruthTableT,
         typename OutputTuple>
class GateTemplate
{
    static_assert("You need to use TypePack to wrap the input and output pin type");
};

template <typename... Ts>
class TypePack {};
template <typename... InputPinT,
         typename TruthTableT,
         typename... OutputPinT>
class GateTemplate <TypePack<InputPinT...>,
      TruthTableT,
      TypePack<OutputPinT...>> : public IGate
{
public:
    virtual void compute() override
    {
    }
protected:
    static const TruthTableT m_truthTable;
};

} // namespace gate
#endif
