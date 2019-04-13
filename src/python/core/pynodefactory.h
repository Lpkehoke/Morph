#pragma once

#include "core/nodefactory.h"
#include "core/nodestoragetypes.h"

class PyNodeFactory : public core::NodeFactory
{
  public:
    virtual core::NodePtr create(const core::Dict& params) const override;
    virtual core::Dict node_info() const override;
};
