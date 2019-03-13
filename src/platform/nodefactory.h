#pragma once

#include <string>

namespace platform { class Node; }

namespace platform
{

class NodeFactory
{
  public:
    virtual Node*       create() const = 0;
    virtual std::string model() const  = 0;

    virtual ~NodeFactory() = default;
};

} // namespace platform
