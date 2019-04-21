#pragma once

#include "ui/windowmanager.h"

#include "foundation/taskqueue.h"

#include <atomic>
#include <thread>

namespace ui
{

class Application
{
  public:
    Application();
    ~Application();

    //void update_root_component(const Component* root_component);

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
};

} // namespace ui
