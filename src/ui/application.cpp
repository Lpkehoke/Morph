#include "ui/application.h"

#include <SDL2/SDL.h>

#include <cassert>
#include <stack>

namespace ui
{

Application::Application()
  : m_app_status(ApplicationStatus::Running)
  , m_queue(foundation::TaskQueue::Priority::High)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        throw 0;
    }

    m_queue.post(
        [=]()
        {
            poll_event();
        });
};

void Application::poll_event()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    }

    if (m_app_status == ApplicationStatus::Running)
    {
        m_queue.post(
            [=]()
            {
                poll_event();
            });
    }
}

// void Application::render_component(const Component* root_component)
// {
//     std::stack<const Component*> comp_stack;

//     // Zero-level components. Each component is rendered in a separate window.
// }

Application::~Application()
{
    assert(m_app_status == ApplicationStatus::Stopped);
    SDL_Quit();
}

} // namespace ui
