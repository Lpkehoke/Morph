#pragma once

#include "nodestoragetypes.h"

#include "base/immutable/map.h"

#include <variant>
#include <string>


namespace platform
{

class Knob
{
  public:
    using AttrMap = immutable::Map<std::string, AttrId>;

    AttrId attribute(const std::string& attr_name) const;
    
    NodeId owner() const;

    bool        is_reference() const;
    bool        accept(const Knob& other) const;
    KnobPtr     connect(const NodeId node_id, const std::string& knob_name);

    AttrMap::Iterator begin() const;
    AttrMap::Iterator end() const;

  private:
    struct KnobRef
    {
        NodeId      node_id;
        std::string knob_name;
    };

    using Value = std::variant<AttrMap, KnobRef>;

    Knob(NodeId owner, KnobRef&& knob_ref);

    NodeId  m_owner;
    Value   m_value;
};

} // namespace platform
