#pragma once

#include "platform/node.h"
#include "platform/nodestoragetypes.h"

class PyNode : public platform::Node
{
  public:
    using Node::Node;
    
    virtual void compute() const override;
};
