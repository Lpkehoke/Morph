#pragma once

#include "ui/uitypes.h"

#include <map>
#include <memory>

namespace ui { class Component; }
namespace ui { class Window; }

namespace ui
{

using WindowPtr = std::shared_ptr<Window>;

class WindowManager
{
  public:
    //WindowPtr get_window_for(const Component* root_component);

  private:
    std::map<ComponentId, WindowPtr> m_windows;
};

} // namespace ui
