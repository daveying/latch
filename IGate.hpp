#ifndef IGATE_HPP__
#define IGATE_HPP__

#include <IPin.hpp>

namespace gate
{

using GateID = size_t;

class IGate
{
public:
    virtual void compute()             = 0;
    virtual IPin* input(size_t index)  = 0;
    virtual IPin* output(size_t index) = 0;
};

template <GateID ID>
class gate_info
{
    static_assert("Gate ID invalid");
};

} // namespace gate
#endif
