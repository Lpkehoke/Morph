#pragma once

#include "core/enabledispatch.h"

namespace computeengine
{

//
//  ComputeEngine is responsible for updating Node
//  output attributes on demand.
//
class ComputeEngine : public core::EnableDispatch
{
  public:
    ComputeEngine(const core::EnableDispatch& enable_dispatch);

    void on_update() const;
};

} // namespace computeengine