#ifndef COMPONENT_BASE_HPP__
#define COMPONENT_BASE_HPP__

#include <string>
#include <vector>
#include <ComponentDescription.hpp>
#include <ComponentFactory.hpp>
#include <ForwardPin.hpp>

namespace component
{

class ComponentBase : public IComponent
{
public:
    ComponentBase(const ComponentDescription& description, const std::string& name)
        : m_name(name)
        , m_description(description)
    {
        for (size_t i = 0; i < m_description.pins.size(); ++i)
        {
            auto& pin = m_description.pins[i];
            if (pin.direction == PinDirection::INPUT)
            {
                m_pins.push_back(std::make_unique<ForwardInputPin>(this, i));
            }
            else
            {
                m_pins.push_back(std::make_unique<ForwardOutputPin>(this, i));
            }
        }
        for (auto const& subcomponent : m_description.subcomponents)
        {
            m_subcomponents.push_back(ComponentFactory::create(subcomponent.type, subcomponent.name));
        }
        for (auto const& connection : m_description.connections)
        {
            if (connection.src.componentIndex == -1 && connection.dest.componentIndex == -1)
            {
                throw std::runtime_error("ComponentBase: Invalid connection, two component pins connected together inside");
            }
            IComponent* srcComp       = subcomponent(connection.src.componentIndex);
            IComponent* destComp      = subcomponent(connection.dest.componentIndex);
            gate::ISourcePin* srcPin  = dynamic_cast<gate::ISourcePin*>(srcComp->pin(connection.src.pinIndex));
            gate::IPin* destPin       = destComp->pin(connection.dest.pinIndex);
            if (srcPin == nullptr)
            {
                throw std::runtime_error("Source pin is not ISourcePin");
            }
            srcPin->connect(destPin);
        }
    }

    virtual gate::IPin* pin(size_t idx) override
    {
        return m_pins[idx].get();
    };

    virtual void initialize() override
    {
        for (auto& subcomp : m_subcomponents)
        {
            subcomp->initialize();
        }
    };

    virtual const std::string& name() const override
    {
        return m_name;
    };

    virtual IComponent* subcomponent(int idx) override
    {
        return (idx == -1) ? this : m_subcomponents[idx].get();
    }

    const ComponentDescription& description()
    {
        return m_description;
    }

protected:
    std::string m_name;
    std::vector<std::unique_ptr<IComponent>> m_subcomponents;
    std::vector<std::unique_ptr<gate::ISourcePin>> m_pins;
    const ComponentDescription& m_description;
};
} // namespace component
#endif // COMPONENT_BASE_HPP__
