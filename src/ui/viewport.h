#pragma once

#include "core/nodestoragetypes.h"

#include <memory>

namespace ui
{

class Viewport
{
  public:
    Viewport();
    void render(core::NodeStorageState state);
  
  private:
};

} // namespace ui
