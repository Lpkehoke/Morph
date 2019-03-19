#pragma once

#include "platform/enabledispatch.h"

namespace computeengine
{

//
//  ComputeEngine is responsible for updating Node
//  output attributes on demand.
//
class ComputeEngine : public platform::EnableDispatch
{
  public:
    ComputeEngine(const platform::EnableDispatch& enable_dispatch);

    void on_update() const;
};

} // namespace computeengine