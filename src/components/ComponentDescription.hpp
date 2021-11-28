/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2021 Xingpeng Da
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/////////////////////////////////////////////////////////////////////////////////

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
    std::string type; // the type of the subcomponent, has to be the ones registered into the factory
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

enum class ConnectionDefinitionType
{
    SINGLE_2_SINGLE,
    SINGLE_2_COMPONENT_ARRAY,
    SINGLE_2_PIN_ARRAY,
    PIN_ARRAY_2_PIN_ARRAY,
    COMPONENT_ARRAY_2_PIN_ARRAY,
    PIN_ARRAY_2_COMPONENT_ARRAY,
    COMPONENT_ARRAY_2_COMPONENT_ARRAY
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

template <size_t I>
static inline constexpr size_t int_digits()
{
    size_t digits = 0;
    size_t number = I;
    while(number > 0)
    {
        number /= 10;
        digits++;
    }
    return digits;
}

template <size_t I, size_t LEN>
static inline constexpr const char* int_str(char (&ret)[LEN])
{
    constexpr size_t STR_LEN = int_digits<I>() + 1;
    static_assert(LEN < STR_LEN, "Not enough space for ret");
    size_t number = I;
    ret[STR_LEN - 1] = '\0';
    size_t index = 2;
    while (number > 0)
    {
        ret[STR_LEN - index] = '0' + static_cast<char>(number % 10);
        number /= 10;
        index++;
    }
    return ret;
}

template <size_t NUM, size_t LEN, size_t RET_LEN>
static inline constexpr const char* stringWith(const char (&str)[LEN], char (&ret)[RET_LEN])
{
    constexpr size_t RET_SIZE = LEN + int_digits<NUM>(); // LEN includes trailing \0
    static_assert(RET_LEN < RET_SIZE, "Not enough space for ret");
    ret[RET_SIZE - 1] = '\0';
    for (size_t i = 0; i < LEN - 1; ++i)
    {
        ret[i] = str[i];
    }
    char numStr[int_digits<NUM>() + 1];
    int_str<NUM>(numStr);
    for (size_t i = 0; i < int_digits<NUM>(); ++i)
    {
        ret[LEN + i] = numStr[i];
    }
    return ret;
}

template <size_t IDX = 0, typename FuncT, typename... Tp>
static inline typename std::enable_if<IDX == sizeof...(Tp)>::type
for_each(std::tuple<Tp...>&, FuncT) {}

template <size_t IDX = 0, typename FuncT, typename... Tp>
static inline typename std::enable_if<IDX < sizeof...(Tp)>::type
for_each(std::tuple<Tp...>& t, FuncT f)
{
    f(std::get<IDX>(t), IDX);
    for_each<IDX + 1, FuncT, Tp...>(t, f);
}

static constexpr size_t PIN_NAME = 0;
static constexpr size_t PIN_TYPE = 1;
static constexpr size_t PIN_SIZE = 2;

static constexpr const char defaultInputNames[][5]{"in0", "in1", "in2", "in3", "in4", "in5", "in6", "in7", "in8", "in9", "in10"};
static constexpr const char defaultOutputNames[][6]{"out0", "out1", "out2", "out3", "out4", "out5", "out6", "out7", "out8", "out9", "out10"};

template <typename T>
static PinDirection pinDirection(T*)
{
    return std::remove_pointer<T>::type::direction();
}


template <typename T, size_t... I>
static constexpr auto gatePinsImpl(T& t, size_t inputPinSize, std::index_sequence<I...>)
{
    return std::make_tuple(
        std::make_tuple(
            static_cast<const char*>(
                pinDirection(std::get<I>(t)) == PinDirection::INPUT ? detail::defaultInputNames[I] : detail::defaultOutputNames[I - inputPinSize]
            ),
            std::get<I>(t),
            1
        )...
    );
}

template <typename... Ts>
static constexpr auto gatePins(size_t inputPinSize)
{
    auto t = std::make_tuple(static_cast<Ts*>(nullptr)...);
    return gatePinsImpl(t, inputPinSize, std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename T>
static void getPins(std::vector<PinDescription>& pinsOut)
{
    auto pinsTuple = T::Pins();
    for_each(pinsTuple, [&pinsOut] (auto& pinTuple, size_t) {
            std::string pinName    = std::get<PIN_NAME>(pinTuple);
            PinDirection direction = pinDirection(std::get<PIN_TYPE>(pinTuple));
            size_t pinSize         = std::get<PIN_SIZE>(pinTuple);
            if (pinSize > 1)
            {
                for (size_t i = 0; i < pinSize; ++i)
                {
                    pinsOut.push_back({.name = pinName + "[" + std::to_string(i) + "]", .direction = direction});
                }
            }
            else
            {
                pinsOut.push_back({.name = pinName, .direction = direction});
            }
        });
}

static constexpr size_t SUBCOMP_NAME = 0;
static constexpr size_t SUBCOMP_TYPE = 1;
static constexpr size_t SUBCOMP_SIZE = 2;

template <typename T>
static std::string subcomponentTypeName(T*)
{
    return std::remove_pointer<T>::type::Name();
}

template <typename T>
static void getSubcomponents(std::vector<SubcomponentDescription>& subcomponentsOut)
{
    auto subcompsTuple = T::Subcomponents();
    for_each(subcompsTuple, [&subcomponentsOut] (auto& subcompTuple, size_t) {
            std::string subcompName = std::get<SUBCOMP_NAME>(subcompTuple);
            std::string subcompType = subcomponentTypeName(std::get<SUBCOMP_TYPE>(subcompTuple));
            size_t subcompSize      = std::get<SUBCOMP_SIZE>(subcompTuple);
            if (subcompSize > 1)
            {
                for (size_t i = 0; i < subcompSize; ++i)
                {
                    subcomponentsOut.push_back({.name = subcompName + "[" + std::to_string(i) + "]", .type = subcompType});
                }
            }
            else
            {
                subcomponentsOut.push_back({.name = subcompName, .type = subcompType});
            }
        });
}

template <typename T>
static inline size_t findIndex(const std::vector<T>& vec, const std::string& t, const std::string& msg)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i].name == t)
        {
            return i;
        }
    }
    throw std::runtime_error(msg);
}

static inline bool splitName(const std::string& endpointName, std::string& compName, std::string& pinName)
{
    auto found = endpointName.find('.');
    if (found != std::string::npos)
    {
        compName = endpointName.substr(0, found);
        pinName  = endpointName.substr(found + 1, endpointName.size() - found - 1);
        return true;
    }
    pinName = endpointName;
    return false;
}

static void getEndpoint(const std::string& name,
        const std::vector<std::vector<PinDescription>>& subcompPins,
        const std::vector<PinDescription>& thisPins,
        const std::vector<SubcomponentDescription>& subcomps,
        Endpoint& endpoint)
{
    std::string compName, pinName;
    if (splitName(name, compName, pinName))
    {
        endpoint.componentIndex = static_cast<int>(findIndex(subcomps, compName, "Component not found: " + compName));
        endpoint.pinIndex = findIndex(subcompPins[endpoint.componentIndex], pinName, "Component pin not found: " + name);
    }
    else
    {
        endpoint.componentIndex = -1;
        endpoint.pinIndex = findIndex(thisPins, pinName, "Component pin not found: " + name);
    }
}

static void getEndpointArray(const std::string& name,
        const std::vector<std::vector<PinDescription>>& subcompPins,
        const std::vector<PinDescription>& thisPins,
        const std::vector<SubcomponentDescription>& subcomps,
        size_t arraySize,
        bool isPinArray,   // true if is pin array, false is component array
        std::vector<Endpoint>& endpoints)
{
    std::string compName, pinName;
    if (splitName(name, compName, pinName))
    {
        if (isPinArray)
        {
            for (size_t i = 0; i < arraySize; ++i)
            {
                std::string pinWithIdx = pinName + "[" + std::to_string(i) + "]";
                Endpoint t;
                t.componentIndex = static_cast<int>(findIndex(subcomps, compName, "Dest component not found: " + compName));
                t.pinIndex = findIndex(subcompPins[t.componentIndex], pinWithIdx, "Source component pin not found: " + compName + "." + pinWithIdx);
                endpoints.push_back(t);
            }
        }
        else
        {
            for (size_t i = 0; i < arraySize; ++i)
            {
                std::string compWithIdx = compName + "[" + std::to_string(i) + "]";
                Endpoint t;
                t.componentIndex = static_cast<int>(findIndex(subcomps, compWithIdx, "Dest component not found: " + compWithIdx));
                t.pinIndex = findIndex(subcompPins[t.componentIndex], pinName, "Source component pin not found: " + compWithIdx + "." + pinName);
                endpoints.push_back(t);
            }
        }
    }
    else
    {
        if (isPinArray)
        {
            for (size_t i = 0; i < arraySize; ++i)
            {
                std::string pinWithIdx = pinName + "[" + std::to_string(i) + "]";
                Endpoint t;
                t.componentIndex = -1;
                t.pinIndex = findIndex(thisPins, pinWithIdx, "Source component pin not found: " + compName + "." + pinWithIdx);
                endpoints.push_back(t);
            }
        }
        else
        {
            throw std::runtime_error("Endpoint array respect to subcomponents, but the component is this component");
        }
    }
}

template <typename T>
static void getConnections(ComponentDescription& desc)
{
    auto subcompsTuple    = T::Subcomponents();
    auto connectionsTuple = T::Connections();
    std::vector<std::vector<PinDescription>> subcompPins;
    subcompPins.reserve(std::tuple_size<decltype(subcompsTuple)>::value);
    for_each(subcompsTuple, [&subcompPins] (auto& subcompTuple, size_t idx) {
            auto& subPins  = subcompPins[idx];
            using SubcompT = typename std::remove_pointer<typename std::remove_reference<decltype(std::get<SUBCOMP_TYPE>(subcompTuple))>::type>::type;
            getPins<SubcompT>(subPins);
        });
    for_each(connectionsTuple, [&] (auto& connectionTuple, size_t) {
            auto& thisPins = desc.pins;
            auto& subcomps = desc.subcomponents;
            switch (std::get<0>(connectionTuple))
            {
            case ConnectionDefinitionType::SINGLE_2_SINGLE: // single to single
            {
                std::string from     = std::get<1>(connectionTuple);
                std::string to       = std::get<2>(connectionTuple);
                Endpoint f;
                Endpoint t;
                getEndpoint(from, subcompPins, thisPins, subcomps, f);
                getEndpoint(to, subcompPins, thisPins, subcomps, t);
                desc.connections.push_back({.src = f, .dest = t});
            }
            case ConnectionDefinitionType::SINGLE_2_COMPONENT_ARRAY:
            case ConnectionDefinitionType::SINGLE_2_PIN_ARRAY:
            {
                auto type        = std::get<0>(connectionTuple);
                std::string from = std::get<1>(connectionTuple);
                std::string to   = std::get<2>(connectionTuple);
                size_t toSize    = std::get<3>(connectionTuple);
                Endpoint f;
                std::vector<Endpoint> ts;
                bool isPinArray = type == ConnectionDefinitionType::SINGLE_2_PIN_ARRAY;
                getEndpoint(from, subcompPins, thisPins, subcomps, f);
                getEndpointArray(to, subcompPins, thisPins, subcomps, toSize, isPinArray, ts);
                for (size_t i = 0; i < ts.size(); ++i)
                {
                    desc.connections.push_back({.src = f, .dest = ts[i]});
                }
            }
            case ConnectionDefinitionType::PIN_ARRAY_2_PIN_ARRAY:
            case ConnectionDefinitionType::PIN_ARRAY_2_COMPONENT_ARRAY:
            case ConnectionDefinitionType::COMPONENT_ARRAY_2_PIN_ARRAY:
            case ConnectionDefinitionType::COMPONENT_ARRAY_2_COMPONENT_ARRAY:
            {
                auto type        = std::get<0>(connectionTuple);
                std::string from = std::get<1>(connectionTuple);
                std::string to   = std::get<2>(connectionTuple);
                size_t arrSize   = std::get<3>(connectionTuple);
                std::vector<Endpoint> fs;
                std::vector<Endpoint> ts;
                bool isSrcPinArray  = (type == ConnectionDefinitionType::PIN_ARRAY_2_PIN_ARRAY ||
                        type == ConnectionDefinitionType::PIN_ARRAY_2_COMPONENT_ARRAY);
                bool isDestPinArray = (type == ConnectionDefinitionType::PIN_ARRAY_2_PIN_ARRAY ||
                        type == ConnectionDefinitionType::COMPONENT_ARRAY_2_PIN_ARRAY);
                getEndpointArray(from, subcompPins, thisPins, subcomps, arrSize, isSrcPinArray, fs);
                getEndpointArray(to, subcompPins, thisPins, subcomps, arrSize, isDestPinArray, ts);
                for (size_t i = 0; i < ts.size(); ++i)
                {
                    desc.connections.push_back({.src = fs[i], .dest = ts[i]});
                }
            }
            default:
            {
                throw std::runtime_error("Wrong connection tuple");
            }
            }
        });
}

template <typename T>
static ComponentDescription getDescription()
{
    ComponentDescription desc;
    desc.type = T::Name();
    detail::getPins<T>(desc.pins);
    detail::getSubcomponents<T>(desc.subcomponents);
    detail::getConnections<T>(desc);
    desc.createFunc = T::create;
    return desc;
};
} // namespace detail

#define DEFINE_PIN_ARRAY(NAME, TYPE, SIZE) \
    std::make_tuple(NAME, static_cast<TYPE*>(nullptr), SIZE)
#define DEFINE_PIN(NAME, TYPE) DEFINE_PIN_ARRAY(NAME, TYPE, 1)
#define DEFINE_SUBCOMPONENT_ARRAY(NAME, TYPE, SIZE) \
    std::make_tuple(NAME, static_cast<TYPE*>(nullptr), SIZE)
#define DEFINE_SUBCOMPONENT(NAME, TYPE) DEFINE_SUBCOMPONENT_ARRAY(NAME, TYPE, 1)

#define CONNECT(FROM, TO) \
    std::make_tuple(ConnectionDefinitionType::SINGLE_2_SINGLE, FROM, TO, 0)   // will not use 0
#define CONNECT_MULTICAST_COMPONENT(FROM, TO, SIZE) \
    std::make_tuple(ConnectionDefinitionType::SINGLE_2_COMPONENT_ARRAY, FROM, TO, SIZE)
#define CONNECT_MULTICAST_PIN(FROM, TO, SIZE) \
    std::make_tuple(ConnectionDefinitionType::SINGLE_2_PIN_ARRAY, FROM, TO, SIZE)
#define CONNECT_PIN_ARRAY_2_COMPONENT_ARRAY(FROM, TO, SIZE) \
    std::make_tuple(ConnectionDefinitionType::PIN_ARRAY_2_COMPONENT_ARRAY, FROM, TO, SIZE)
#define CONNECT_PIN_ARRAY_2_PIN_ARRAY(FROM, TO, SIZE) \
    std::make_tuple(ConnectionDefinitionType::PIN_ARRAY_2_PIN_ARRAY, FROM, TO, SIZE)
#define CONNECT_COMPONENT_ARRAY_2_COMPONENT_ARRAY(FROM, TO, SIZE) \
    std::make_tuple(ConnectionDefinitionType::COMPONENT_ARRAY_2_COMPONENT_ARRAY, FROM, TO, SIZE)
#define CONNECT_COMPONENT_ARRAY_2_PIN_ARRAY(FROM, TO, SIZE) \
    std::make_tuple(ConnectionDefinitionType::COMPONENT_ARRAY_2_PIN_ARRAY, FROM, TO, SIZE)
} // namespace component

#endif // COMPONENT_DESCRIPTION_HPP__
