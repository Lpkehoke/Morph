#pragma once

#include "base/immutable/map.h"

#include <cstdint>
#include <memory>
#include <string>
#include <variant>

namespace platform { class Attribute; }
namespace platform { class Knob; }
namespace platform { class Node; }

namespace platform
{

using AttrId = std::uint64_t;
using KnobId = std::uint64_t;
using NodeId = std::uint64_t;

using AttrPtr = std::shared_ptr<Attribute>;
using KnobPtr = std::shared_ptr<Knob>;
using NodePtr = std::shared_ptr<Node>;

using AttributeCollection   = immutable::Map<AttrId, AttrPtr>;
using KnobCollection        = immutable::Map<KnobId, KnobPtr>;
using NodeCollection        = immutable::Map<NodeId, NodePtr>;

using MetadataProperty      = std::variant<std::string, double, bool>;
using Metadata              = immutable::Map<std::string, MetadataProperty>;
using MetadataCollection    = immutable::Map<NodeId, Metadata>;

struct NodeStorageState
{
    NodeStorageState()
        : m_next_node_id(0)
        , m_next_knob_id(0)
        , m_next_attr_id(0)
    {}
    
    AttributeCollection     m_attributes;
    KnobCollection          m_knobs;
    NodeCollection          m_nodes;
    MetadataCollection      m_node_metadata;
    MetadataCollection      m_knob_metadata;
    NodeId                  m_next_node_id;
    KnobId                  m_next_knob_id;
    AttrId                  m_next_attr_id;
};

} // namespace platform
