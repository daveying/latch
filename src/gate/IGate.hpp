#ifndef IGATE_HPP__
#define IGATE_HPP__

#include <cstddef>
#include <string>
#include <IPin.hpp>

namespace gate
{

using GateID = size_t;

class IGate
{
public:
    virtual void compute()                   = 0;
    virtual IPin* input(size_t index)        = 0;
    virtual ISourcePin* output(size_t index) = 0;
    virtual const std::string& name() const  = 0;
};

class TruthTableBase {};
} // namespace gate
#endif
