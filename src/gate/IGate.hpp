#ifndef IGATE_HPP__
#define IGATE_HPP__

#include <cstddef>
#include <string>
#include <IPin.hpp>

namespace component
{
class IComponent
{
public:
    virtual void initialize()                 = 0;
    virtual IComponent* subcomponent(int idx) = 0;
    virtual gate::IPin* pin(size_t idx)       = 0;
    virtual const std::string& name() const   = 0;
};
} // namespace component

namespace gate
{
class IGate : public component::IComponent
{
public:
    virtual void compute()                   = 0;
    virtual IPin* input(size_t index)        = 0;
    virtual ISourcePin* output(size_t index) = 0;
    virtual void initialize() final {}
    virtual component::IComponent* subcomponent(int) final
    {
        return nullptr;
    };
};

class TruthTableBase {};
} // namespace gate
#endif
