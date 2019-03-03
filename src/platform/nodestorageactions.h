#pragma once

#include "node.h"

#include <cstdint>
#include <string>
#include <variant>

namespace platform
{

struct CreateNode
{
    NodeId      id;
    std::string model;
};

struct RemoveNode
{
    NodeId id;
};

using NodeStorageAction = std::variant<
    CreateNode,
    RemoveNode>;

} // namespace platform
