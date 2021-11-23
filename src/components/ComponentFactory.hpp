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

#include <ComponentDescription.hpp>

namespace component
{

class ComponentFactory
{
public:
    static void clear();
    template <typename T>
    static void registerPrecompiledComponent(const std::string& type)
    {
        if (m_preCompiledComponents.find(type) != m_preCompiledComponents.end())
        {
            throw std::runtime_error("Precompiled component type: " + type + " has already been registered.");
        }
        ComponentDescription& desc = m_preCompiledComponents[type];
        desc.type = type;
        detail::getPins<T>(desc.pins);
        detail::getSubcomponents<T>(desc.subcomponents);
        detail::getConnections<T>(desc.connections);
        desc.createFunc = T::create;
    }
    static void registerCustomComponents(const ComponentDescription& desc);
    static std::unique_ptr<IComponent> create(const std::string& type, const std::string& name);
private:
    static std::unordered_map<std::string, ComponentDescription> m_preCompiledComponents;
    static std::unordered_map<std::string, ComponentDescription> m_customComponents;
};

} // namespace component

#endif // COMPONENT_FACTORY_HPP__


