#ifndef REGISTER_HPP__
#define REGISTER_HPP__

#include <IComponent.hpp>

namespace component
{

template <size_t BITS>
class Register : public IComponent
{
public:
    virtual ~Register() {}
protected:
};

} // namespace component

#endif // REGISTER_HPP__
