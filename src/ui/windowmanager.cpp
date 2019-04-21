#include "ui/windowmanager.h"

#include "ui/window.h"

#include <memory>
#include <utility>

namespace ui
{

// WindowPtr WindowManager::get_window_for(const Component* root_component)
// {
//     auto itr = m_windows.find(root_component->id());

//     if (itr != m_windows.end())
//     {
//         auto window = itr->second;
//         window->set_title(root_component->window_title());

//         return std::move(window);
//     }

//     WindowPtr window = std::make_shared<Window>(
//         root_component->id(),
//         root_component->window_title());

//     m_windows.emplace(root_component->id(), window);

//     return std::move(window);
// }

} // namespace ui
