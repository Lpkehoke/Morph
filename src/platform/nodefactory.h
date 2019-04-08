#pragma once

#include "nodestoragetypes.h"
#include "node.h"

#include <map>
#include <string>

namespace platform
{

class NodeFactory
{
  public:
    struct NodeKnobsInfo
    {
        std::map<std::string, std::string> input_knobs_schema;
        std::map<std::string, std::string> output_knobs_schema;
    };

    virtual NodePtr create(
        Node::KnobMap input_knobs,
        Node::KnobMap output_knobs) const = 0;
    
    virtual const std::string& model() const = 0;
    virtual const NodeKnobsInfo& knobs_info() const = 0;

    virtual ~NodeFactory() = default;
};

} // namespace platform
