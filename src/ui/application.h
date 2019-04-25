#pragma once

#include "ui/component.h"
#include "ui/rendertree.h"
#include "ui/windowmanager.h"

#include "foundation/taskqueue.h"

#include <atomic>
#include <thread>

namespace ui { class ComponentFactory; }

namespace ui
{

class Application
{
  public:
    Application();
    ~Application();

    void render(const ComponentFactory* component_factory, core::Dict props);

  private:
    enum class ApplicationStatus
    {
        Running,
        Stopped
    };

    void poll_event();

    ApplicationStatus       m_app_status;
    foundation::TaskQueue   m_queue;
    WindowManager           m_window_manager;
    ElementPtr              m_render_tree;
    ElementId               m_next_element_id;
};

} // namespace ui
