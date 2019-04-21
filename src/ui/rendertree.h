#pragma once

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
    Window,
    DrawCommand
};

struct Element;
using ElementId = std::uint64_t;
using ElementPtr = std::shared_ptr<Element>;
using ElementChildrenCollection = foundation::Map<ElementId, ElementPtr>;


struct ElementInitializer
{
    ElementType                 type;
    ComponentFactory*           component_factory;
};

struct Element
{
    ElementType                 type;
    Component*                  component;
    std::optional<int>          output;
    ElementChildrenCollection   children;
};

} // namespace ui
