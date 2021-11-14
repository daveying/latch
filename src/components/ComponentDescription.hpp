#ifndef COMPONENT_DESCRIPTION_HPP__
#define COMPONENT_DESCRIPTION_HPP__

#include <string>
#include <functional>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <IComponent.hpp>

namespace component
{

enum class PinDirection
{
    INPUT,
    OUTPUT,
    VIRTUAL,
};

struct PinDescription
{
    std::string name;
    PinDirection direction;
};

// only the type name is referenced, so has to come up with
// this subcomponent description struct
struct SubcomponentDescription
{
    std::string name; // the name of the subcomponent, can be empty
    std::string type; // the type ofthe subcomponent, has to be the ones registered into the factory
};

struct Endpoint
{
    int componentIndex;
    size_t pinIndex;
};

struct ConnectionDescription
{
    Endpoint src;
    Endpoint dest;
};

// POD describes any type of component
struct ComponentDescription
{
    std::string type; // type name of this component
    std::vector<PinDescription> pins;
    std::vector<SubcomponentDescription> subcomponents; // including gates and other component
    std::vector<ConnectionDescription> connections;
    std::function<std::unique_ptr<IComponent>(const std::string&)> createFunc;
};

namespace detail
{

static constexpr size_t PIN_NAME = 0;
static constexpr size_t PIN_TYPE = 1;

template <typename T>
void getPins(std::vector<PinDescription>& pins) {};

template <typename T>
void getSubcomponents(std::vector<SubcomponentDescription>& subcomponents) {};

template <typename T>
void getConnections(std::vector<ConnectionDescription>& connections) {};

} // namespace detail
} // namespace component

#endif // COMPONENT_DESCRIPTION_HPP__
