#pragma once

#include "ui/rendertree.h"

#include <functional>
#include <string>

namespace core { class Dict; }

namespace ui
{

class Component
{
  public:
    virtual core::Dict initialize_state(const core::Dict& props) const = 0;

    virtual ElementInitializerCollection render() const = 0;

    virtual bool should_component_update(
        const core::Dict& new_props,
        const core::Dict& new_state) const = 0;

    virtual std::string model() const = 0;

    virtual ~Component() = default;
};

class ComponentFactory
{
  public:
    virtual Component* create() const = 0;
    virtual std::string model() const = 0;
    virtual ~ComponentFactory() = default;
};

} // namespace ui
