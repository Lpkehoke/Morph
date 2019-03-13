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

using NodeStorageAction = std::variant<
    CreateNode,
    RemoveNode,
    UpdateNodeMetadata>;

} // namespace platform
