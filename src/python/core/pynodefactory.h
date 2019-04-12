#pragma once

#include "core/nodefactory.h"
#include "core/nodestoragetypes.h"

class PyNodeFactory : public core::NodeFactory
{
  public:
    virtual core::NodePtr create(
        core::Node::KnobMap input_knobs,
        core::Node::KnobMap output_knobs) const override;
    
    virtual const std::string& model() const override;
    virtual const NodeKnobsInfo& knobs_info() const override;
};
