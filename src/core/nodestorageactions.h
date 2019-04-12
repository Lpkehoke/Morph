#pragma once

#include "core/dict.h"
#include "core/node.h"
#include "core/nodestoragetypes.h"

#include <cstdint>
#include <string>
#include <variant>

namespace core
{

struct CreateNode
{
    std::string         model;
    Dict                metadata;
};

struct RemoveNode
{
    NodeId              id;
};

struct UpdateNodeMetadata
{
    NodeId              id;
    Dict                metadata;
};

struct MakeConnection
{
    NodeId              output_node_id;
    std::string         output_knob_name;
    NodeId              input_node_id;
    std::string         input_knob_name;
};

struct BreakConnection
{
    NodeId              input_node_id;
    std::string         input_knob_name;
};

struct UpdateAttribute
{
    AttrId              id;
    AttrPtr             value;
};

using NodeStorageAction = std::variant<
    CreateNode,
    RemoveNode,
    UpdateNodeMetadata,
    MakeConnection,
    BreakConnection,
    UpdateAttribute>;

} // namespace core
