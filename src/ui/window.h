#pragma once

#include "uitypes.h"

#include <string>

namespace ui
{

class Window
{
  public:
    Window(ComponentId root_component_id, const std::string& title);
    ~Window();

    void set_title(const std::string& title);

  private:
    struct Impl;
    Impl* impl;
};

} // namespace ui
