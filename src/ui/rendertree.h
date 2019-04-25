#pragma once

#include "core/dict.h"

#include "ui/uitypes.h"

#include "foundation/immutable/map.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

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
using ElementKey = std::uint64_t;
using ElementPtr = std::shared_ptr<Element>;

using ElementCollection = foundation::Map<ElementKey, ElementPtr>;
using ElementInitializerCollection = std::vector<ElementInitializer>;

struct ElementInitializer
{
    ElementType                 type;
    const ComponentFactory*     component_factory;
    core::Dict                  props;
    ElementKey                  key;
};

struct Element
{
    ElementId                   id;
    ElementType                 type;
    const Component*            component;
    core::Dict                  props;
    core::Dict                  state;
    std::optional<int>          output;
    ElementCollection           children;
};

} // namespace ui
