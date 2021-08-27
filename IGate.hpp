#ifndef IGATE_HPP__
#define IGATE_HPP__

namespace gate
{

using GateID = size_t;

class IGate
{
public:
    virtual void compute() = 0;
};

template <GateID ID>
class gate_info
{
    static_assert("Gate ID invalid");
};

} // namespace gate
#endif
