#ifndef COMPONENT_FACTORY_HPP__
#define COMPONENT_FACTORY_HPP__

#include <ComponentBase.hpp>
#include <ComponentDescription.hpp>

namespace component
{

class ComponentFactory
{
public:
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


