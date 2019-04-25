#pragma once

#include "core/dict.h"

#include "ui/uitypes.h"

#include "foundation/immutable/map.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace ui { class Component; }
namespace ui { class ComponentFactory; }

namespace ui
{

enum class ElementType
{
    Component,
    Root,
    Window,
    DrawCommand
};

struct Element;
struct ElementInitializer;

using ElementId = std::uint64_t;
using ElementPtr = std::shared_ptr<Element>;
using ElementChildrenCollection = foundation::Map<ElementId, ElementPtr>;
using ElementInitializerCollection = foundation::Map<ElementId, ElementInitializer>;

struct ElementInitializer
{
    ElementType                 type;
    ComponentFactory*           component_factory;
    core::Dict                  props;
};

struct Element
{
    ElementType                 type;
    Component*                  component;
    core::Dict                  props;
    core::Dict                  state;
    core::Dict                  last_rendered_state;
    std::optional<int>          output;
    ElementChildrenCollection   children;
};

} // namespace ui
