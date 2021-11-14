#ifndef COMPONENT_BASE_HPP__
#define COMPONENT_BASE_HPP__

#include <string>
#include <vector>
#include <ComponentDescription.hpp>
#include <ComponentFactory.hpp>

namespace component
{

class ComponentBase : public IComponent
{
public:
    ComponentBase(const ComponentDescription& description, const std::string& name)
        : m_name(name)
        , m_description(description)
    {
        for (auto const& subcomponent : m_description.subcomponents)
        {
            m_subcomponents.push_back(ComponentFactory::create(subcomponent.type, subcomponent.name));
        }
        for (auto const& connection : m_description.connections)
        {
            // if the connection is from or to this component, skip
            if (connection.src.componentIndex == -1 || connection.dest.componentIndex == -1)
                continue;
            connect(
                m_subcomponents[connection.src.componentIndex]->pin(connection.src.pinIndex),
                m_subcomponents[connection.dest.componentIndex]->pin(connection.dest.pinIndex)
            );
        }
    }
    virtual gate::IPin* pin(size_t idx) override { return nullptr; };
    virtual void initialize() override {};
    virtual const std::string& name() const override { return ""; };

protected:
    void connect(gate::IPin* src, gate::IPin* dest) {};
    std::string m_name;
    std::vector<std::unique_ptr<IComponent>> m_subcomponents;
    const ComponentDescription& m_description;
};
} // namespace component
#endif // COMPONENT_BASE_HPP__
