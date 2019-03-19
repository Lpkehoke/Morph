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

    Knob(NodeId owner, std::string model_name, AttrMap attr_map);

    AttrId attribute(const std::string& attr_name) const;
    
    NodeId owner() const;

    bool        is_reference() const;
    NodeId      referenced_node() const;
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

    using Value = std::variant<std::monostate, AttrMap, KnobRef>;

    Knob(
        std::string model_name,
        NodeId      owner,
        KnobRef&&   knob_ref);

    NodeId          m_owner;
    std::string     m_model_name;
    Value           m_value;
};

} // namespace platform
