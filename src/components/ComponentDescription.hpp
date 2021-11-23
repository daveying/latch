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

enum class PinDirection
{
    INPUT,
    OUTPUT,
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
