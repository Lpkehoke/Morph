#pragma once

#include "core/node.h"
#include "core/nodestoragetypes.h"

class PyNode : public core::Node
{
  public:
    using Node::Node;
    
    virtual void compute() const override;
};
