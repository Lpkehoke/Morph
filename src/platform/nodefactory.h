#pragma once

#include "nodestoragetypes.h"

#include <map>
#include <string>

namespace platform
{

class NodeFactory
{
  public:
    virtual NodeStorageState    create(NodeStorageState state) const = 0;
    virtual std::string         model() const = 0;

    virtual ~NodeFactory() = default;
};

} // namespace platform
