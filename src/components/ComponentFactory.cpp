/////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2022 Xingpeng Da
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

#include <set>
#include <ComponentBase.hpp>
#include <ComponentFactory.hpp>

namespace component
{

std::unordered_map<std::string, ComponentDescription> ComponentFactory::m_preCompiledComponents;
std::unordered_map<std::string, ComponentDescription> ComponentFactory::m_customComponents;

void ComponentFactory::clearCustomComponents()
{
    m_customComponents.clear();
}

void ComponentFactory::registerCustomComponent(const ComponentDescription& desc)
{
    if (m_preCompiledComponents.find(desc.type) != m_preCompiledComponents.end())
    {
        throw std::runtime_error("Component type: " + desc.type + " is a pre-compiled component name.");
    }
    if (m_customComponents.find(desc.type) != m_customComponents.end())
    {
        throw std::runtime_error("Custom component type: " + desc.type + " has already been registered.");
    }
    validateDescription(desc);
    m_customComponents[desc.type] = desc;
}

bool ComponentFactory::tryRegisterCustomComponent(const ComponentDescription& desc)
{
    if (m_preCompiledComponents.find(desc.type) != m_preCompiledComponents.end()
        || m_customComponents.find(desc.type) != m_customComponents.end())
    {
        return false;
    }
    validateDescription(desc);
    m_customComponents[desc.type] = desc;
    return true;
}

void ComponentFactory::validateDescription(const ComponentDescription& descriptor)
{
    std::stringstream errInfo;
    if (!isDescriptionValid(descriptor, errInfo))
    {
        throw std::runtime_error("Precompiled component type: " + descriptor.type + " is not valid: " + errInfo.str());
    }
}

bool ComponentFactory::isDescriptionValid(const ComponentDescription& descriptor, std::stringstream& errInfo)
{
    bool valid = true;
    auto checkName = [&errInfo, &valid](auto& vec, std::string vecName) {
        std::set<std::string> names;
        for (const auto& item : vec)
        {
            auto found = names.find(item.name);
            if (found != names.end())
            {
                valid = false;
                errInfo << "duplicate " << vecName << " name '" << *found << "';";
            }
            names.insert(item.name);
        }
    };
    // pin name needs to be unique
    checkName(descriptor.pins, "pin");
    // subcomponent name needs to be unique
    checkName(descriptor.subcomponents, "subcomponent");

    size_t numComps = descriptor.subcomponents.size();
    // connection indices should be valid index
    for (const auto& conn : descriptor.connections)
    {
        // componentIndex = -1 means this component
        if (conn.src.componentIndex >= numComps || conn.src.componentIndex < -1)
        {
            errInfo << "connection src contains invalid component index: '" << conn.src.componentIndex << "';";
        }
        if (conn.dest.componentIndex >= numComps || conn.dest.componentIndex < -1)
        {
            errInfo << "connection dest contains invalid component index: '" << conn.dest.componentIndex << "';";
        }
    }
    // should not contain duplicate connections
    std::map<Endpoint, std::set<Endpoint>> connections;
    for (const auto& conn : descriptor.connections)
    {
        auto srcFound = connections.find(conn.src);
        if (srcFound != connections.end())
        {
            auto& destSet = srcFound->second;
            auto destFound = destSet.find(conn.dest);
            if (destFound != destSet.end())
            {
                valid = false;
                errInfo << "duplicate connection src: (" << conn.src.componentIndex << "," << conn.src.pinIndex
                    << "), dest: (" << conn.dest.componentIndex << "," << conn.dest.pinIndex << ");";
            }
            destSet.insert(conn.dest);
        }
        else
        {
            std::set<Endpoint> destSet;
            destSet.insert(conn.dest);
            connections.insert({conn.src, std::move(destSet)});
        }
    }

    return valid;
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

JSON ComponentFactory::dumpDescription(const ComponentDescription& desc)
{
    JSON ret = JSON::object();
    ret["type"] = desc.type;
    ret["pins"] = JSON::array();
    auto& pins = ret["pins"];
    for (const auto& pin : desc.pins)
    {
        pins.push_back({
            {"type", pin.type},
            {"name", pin.name},
            {"direction", std::to_string(pin.direction)}
        });
    }
    ret["subcomponents"] = JSON::array();
    auto& subcomponents = ret["subcomponents"];
    for (const auto& subcomp : desc.subcomponents)
    {
        subcomponents.push_back({
            {"name", subcomp.name},
            {"type", subcomp.type}
        });
    }

    auto pinName = [&desc](const Endpoint& pin) -> std::string {
        std::string ret;
        if (pin.componentIndex == -1)
        {
            ret = desc.pins[pin.pinIndex].name;
        }
        else
        {
            ret = desc.subcomponents[pin.componentIndex].name + "." + std::to_string(pin.pinIndex);
        }
        return ret;
    };

    ret["connections"] = JSON::array();
    auto& connections = ret["connections"];
    for (const auto& conn : desc.connections)
    {
        connections.push_back({
            {"src", {
                {"name", pinName(conn.src)},
                {"componentIndex", conn.src.componentIndex},
                {"pinIndex", conn.src.pinIndex},
            }},
            {"dest", {
                {"name", pinName(conn.dest)},
                {"componentIndex", conn.dest.componentIndex},
                {"pinIndex", conn.dest.pinIndex},
            }}
        });
    }
    return ret;
}

JSON ComponentFactory::dump()
{
    JSON ret = JSON::object();
    ret["descriptors"] = JSON::object();
    /**
     * The dump format:
     * {
     *   "descriptors": {
     *     "<descriptor ID>": {
     *       "type": "<descriptor ID>",
     *       "pins": [
     *         {
     *           "name": "<pin name>",
     *           "direction": "INPUT|OUTPUT",
     *           "type": "<pin type>"
     *         },
     *         ...
     *       ],
     *       "subcomponents": [
     *         {
     *           "name": "<component name>",
     *           "type": "<descriptor ID>"
     *         },
     *         ...
     *       ],
     *       "connections": [
     *         {
     *           "src": {
     *             "name": "optional: <component name>.<pin name>",
     *             "componentIndex": <component index>,
     *             "pinIndex": <pin index>
     *           },
     *           "dest": {
     *             "name": "optional: <component name>.<pin name>",
     *             "componentIndex": <component index>,
     *             "pinIndex": <pin index>
     *           }
     *         },
     *         ...
     *       ]
     *     }
     *   }
     * }
     */

    using DescMap = std::unordered_map<std::string, ComponentDescription>;
    auto dumpDescVec = [](const DescMap& vec, JSON& ret) {
        for (const auto& desc : vec)
        {
            ret[desc.first] = dumpDescription(desc.second);
        }
    };

    dumpDescVec(m_preCompiledComponents, ret["descriptors"]);
    dumpDescVec(m_customComponents, ret["descriptors"]);

    return ret;
}

} // namespace component
