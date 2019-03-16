#pragma once

#include "node.h"
#include "nodestoragetypes.h"

#include <cstdint>
#include <string>
#include <variant>

namespace platform
{

struct CreateNode
{
    NodeId          id;
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

using NodeStorageAction = std::variant<
    CreateNode,
    RemoveNode,
    UpdateNodeMetadata,
    MakeConnection>;

} // namespace platform
