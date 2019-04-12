#pragma once

#include "core/dict.h"

#include "foundation/immutable/map.h"

#include <cstdint>
#include <memory>
#include <string>
#include <variant>

namespace core { class Attribute; }
namespace core { class Knob; }
namespace core { class Node; }

namespace core
{

using AttrId = std::uint64_t;
using KnobId = std::uint64_t;
using NodeId = std::uint64_t;

using AttrPtr = std::shared_ptr<Attribute>;
using KnobPtr = std::shared_ptr<Knob>;
using NodePtr = std::shared_ptr<Node>;

using AttributeCollection   = foundation::Map<AttrId, AttrPtr>;
using KnobCollection        = foundation::Map<KnobId, KnobPtr>;
using NodeCollection        = foundation::Map<NodeId, NodePtr>;
using MetadataCollection    = foundation::Map<NodeId, Dict>;

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

} // namespace core
