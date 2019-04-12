#pragma once

#include "core/dict.h"
#include "core/node.h"
#include "core/nodestoragetypes.h"

namespace core
{

class NodeFactory
{
  public:
    virtual NodePtr create(
        Node::KnobMap input_knobs,
        Node::KnobMap output_knobs) const = 0;
    
    virtual Dict params() const = 0;

    virtual ~NodeFactory() = default;
};

} // namespace core
