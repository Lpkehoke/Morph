#include "ui/window.h"

#include "ui/uitypes.h"

#include <SDL2/SDL.h>

#include <stdexcept>

namespace ui
{

struct Window::Impl
{
    SDL_Window* m_window;

    // Id of a root component associated with the window.
    ComponentId m_component_id;
};

Window::Window(ComponentId root_component_id, const std::string& title)
  : impl(new Impl())
{
    impl->m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_OPENGL);

    if (!impl->m_window)
    {
        delete impl;
        throw std::runtime_error("Failed to open the window.");
    }

    impl->m_component_id = root_component_id;
    
    // Used to identify window events in application event loop.
    SDL_SetWindowData(
        impl->m_window,
        "component_id",
        static_cast<void*>(&impl->m_component_id));
}

Window::~Window()
{
    if (impl->m_window)
    {
        SDL_DestroyWindow(impl->m_window);
    }

    delete impl;
}

void Window::set_title(const std::string& title)
{
    SDL_SetWindowTitle(impl->m_window, title.c_str());
}

} // namespace ui
