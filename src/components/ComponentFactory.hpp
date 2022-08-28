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

#ifndef COMPONENT_FACTORY_HPP__
#define COMPONENT_FACTORY_HPP__

#include <nlohmann/json.hpp>
#include <ComponentDescription.hpp>

namespace component
{

using JSON = nlohmann::json;

class ComponentFactory
{
public:
    static void clearCustomComponents();

    template <typename T>
    static void registerPrecompiledComponent()
    {
        ComponentDescription desc = detail::getDescription<T>();
        if (m_preCompiledComponents.find(desc.type) != m_preCompiledComponents.end())
        {
            throw std::runtime_error("Precompiled component type: " + desc.type + " has already been registered.");
        }
        m_preCompiledComponents[desc.type] = desc;
    }

    static void registerCustomComponent(const ComponentDescription& desc);
    static bool tryRegisterCustomComponent(const ComponentDescription& desc);
    static std::unique_ptr<IComponent> create(const std::string& type, const std::string& name);
private:
    static std::unordered_map<std::string, ComponentDescription> m_preCompiledComponents;
    static std::unordered_map<std::string, ComponentDescription> m_customComponents;
};

template <typename T>
class ComponentItem
{
public:
    ComponentItem()
    {
        ComponentFactory::registerPrecompiledComponent<T>();
    }
};

// must register in C++ file
#define REGISTER_COMPONENT_WITH_SUFFIX(T, SUFFIX) static component::ComponentItem<T> componentTypeItem##SUFFIX;
#define REGISTER_COMPONENT_WITHOUT_SUFFIX(T) REGISTER_COMPONENT_WITH_SUFFIX(T, T);
#define GET_MACRO(_1, _2, NAME, ...) NAME
#define REGISTER_COMPONENT(...) GET_MACRO(__VA_ARGS__, REGISTER_COMPONENT_WITH_SUFFIX, REGISTER_COMPONENT_WITHOUT_SUFFIX)(__VA_ARGS__)

} // namespace component

#endif // COMPONENT_FACTORY_HPP__


