#pragma once

#include "nodestoragetypes.h"

#include "foundation/immutable/map.h"

#include <memory>
#include <variant>
#include <string>


namespace core
{

class Knob : public std::enable_shared_from_this<Knob>
{
  public:
    using AttrMap = foundation::Map<std::string, AttrId>;

    Knob(NodeId owner, std::string schema_name, AttrMap attr_map);

    AttrId attribute(const std::string& attr_name) const;
    
    NodeId              owner() const;
    const std::string&  model() const;

    bool        is_reference() const;
    NodeId      referenced_node() const;
    bool        accept(const Knob& other) const;
    KnobPtr     connect(const NodeId node_id, const std::string& knob_name);
    KnobPtr     disconnect() const;

    AttrMap::Iterator begin() const;
    AttrMap::Iterator end() const;

  private:
    struct KnobRef
    {
        NodeId      node_id;
        std::string knob_name;
        KnobPtr     disconnected;
    };

    using Value = std::variant<std::monostate, AttrMap, KnobRef>;

    Knob(
        std::string schema_name,
        NodeId      owner,
        KnobRef&&   knob_ref);

    NodeId          m_owner;
    std::string     m_schema_name;
    Value           m_value;
};

} // namespace core
