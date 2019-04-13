#pragma once

#include "core/dict.h"
#include "core/node.h"
#include "core/nodestoragetypes.h"

namespace core
{

class NodeFactory
{
  public:
    virtual NodePtr create(const Dict& params) const = 0;
    virtual Dict node_info() const = 0;

    virtual ~NodeFactory() = default;
};

} // namespace core
