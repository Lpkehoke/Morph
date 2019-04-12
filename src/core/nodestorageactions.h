#pragma once

#include "node.h"
#include "nodestoragetypes.h"

#include <cstdint>
#include <string>
#include <variant>

namespace core
{

struct CreateNode
{
    std::string     model;
    Metadata        metadata;
};

struct RemoveNode
{
    NodeId          id;
};

struct UpdateNodeMetadata
{
    NodeId          id;
    Metadata        metadata;
};

struct MakeConnection
{
    NodeId          output_node_id;
    std::string     output_knob_name;
    NodeId          input_node_id;
    std::string     input_knob_name;
};

struct BreakConnection
{
    NodeId          input_node_id;
    std::string     input_knob_name;
};

struct UpdateAttribute
{
    AttrId          id;
    AttrPtr         value;
};

using NodeStorageAction = std::variant<
    CreateNode,
    RemoveNode,
    UpdateNodeMetadata,
    MakeConnection,
    BreakConnection,
    UpdateAttribute>;

} // namespace core
