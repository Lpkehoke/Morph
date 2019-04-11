#pragma once

#include "platform/nodefactory.h"
#include "platform/nodestoragetypes.h"

class PyNodeFactory : public platform::NodeFactory
{
  public:
    virtual platform::NodePtr create(
        platform::Node::KnobMap input_knobs,
        platform::Node::KnobMap output_knobs) const override;
    
    virtual const std::string& model() const override;
    virtual const NodeKnobsInfo& knobs_info() const override;
};
