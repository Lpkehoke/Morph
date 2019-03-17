#pragma once

#include "nodestoragetypes.h"

#include <map>
#include <string>

namespace platform { class Attribute; }
namespace platform { class Knob; }
namespace platform { class Node; }


namespace platform
{

using DefaultAttributeValueMap = std::map<std::string, AttrPtr>;

struct KnobInitInfo
{
    std::string                 model;
    DefaultAttributeValueMap    default_values;
};

using KnobInitInfoMap = std::map<std::string, KnobInitInfo>;


class NodeFactory
{
  public:
    using KnobsMap = std::map<std::string, KnobId>;

    virtual Node*           create(const KnobsMap& knobs) const = 0;
    virtual std::string     model() const = 0;
    virtual KnobInitInfoMap knobs_init_info() const = 0;

    virtual ~NodeFactory() = default;
};

} // namespace platform
