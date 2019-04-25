#pragma once

#include "ui/rendertree.h"

#include <optional>

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

    virtual ~Component() = default;
};

} // namespace ui
