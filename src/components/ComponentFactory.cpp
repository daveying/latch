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

#include <ComponentBase.hpp>
#include <ComponentFactory.hpp>

namespace component
{

std::unordered_map<std::string, ComponentDescription> ComponentFactory::m_preCompiledComponents;
std::unordered_map<std::string, ComponentDescription> ComponentFactory::m_customComponents;

void ComponentFactory::clear()
{
    m_preCompiledComponents.clear();
    m_customComponents.clear();
}

void ComponentFactory::registerCustomComponents(const ComponentDescription& desc)
{
    if (m_preCompiledComponents.find(desc.type) != m_preCompiledComponents.end())
    {
        throw std::runtime_error("Component type: " + desc.type + " is a pre-compiled component name.");
    }
    if (m_customComponents.find(desc.type) != m_customComponents.end())
    {
        throw std::runtime_error("Custom component type: " + desc.type + " has already been registered.");
    }
    m_customComponents[desc.type] = desc;
}

std::unique_ptr<IComponent> ComponentFactory::create(const std::string& type, const std::string& name)
{
    std::unique_ptr<IComponent> ret;
    auto desc = m_preCompiledComponents.find(type);
    if (desc != m_preCompiledComponents.end())
    {
        ret = desc->second.createFunc(name);
    }
    else
    {
        auto desc2 = m_customComponents.find(type);
        if (desc2 != m_customComponents.end())
        {
            if (desc2->second.createFunc != nullptr)
            {
                ret = desc2->second.createFunc(name);
            }
            else
            {
                ret = std::make_unique<ComponentBase>(desc2->second, name);
            }
        }
    }
    if (ret == nullptr)
    {
        throw std::runtime_error("Cannot find description for " + type);
    }

    return ret;
}

} // namespace component
