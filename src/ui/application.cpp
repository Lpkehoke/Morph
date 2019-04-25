#include "ui/application.h"

#include "ui/component.h"

#include <SDL2/SDL.h>

#include <cassert>
#include <memory>
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

bool keep_element(const ElementPtr& element, const ElementInitializer& element_initializer)
{
    if (element->type != element_initializer.type)
    {
        return false;
    }

    if (element->type == ElementType::Component)
    {
        if (!element->component || !element_initializer.component_factory)
        {
            return false;
        }

        if (element->component->model() != element_initializer.component_factory->model())
        {
            return false;
        }
    }

    return true;
}

bool should_element_update(
    const ElementPtr& element,
    const core::Dict& new_props,
    const core::Dict& new_state)
{
    if ((new_props == element->props) && (new_state == element->state))
    {
        return false;
    }

    if ((element->type == ElementType::Component) && element->component)
    {
        return element->component->should_component_update(new_props, new_state);
    }

    return true;
}

void render_child_element(
    const ElementPtr& parent,
    const ElementInitializer& element_initializer)
{
    if (!parent->children.contains(element_initializer.key))
    {
        // Create new element
        return;
    }

    auto& current_child = parent->children[element_initializer.key];
    if (!keep_element(current_child, element_initializer))
    {
        // create new element
        return;
    }

    bool should_child_update = should_element_update(
        current_child,
        element_initializer.props,
        current_child->state);

    current_child->props = element_initializer.props;

    if (should_child_update)
    {
        ElementInitializerCollection children_initializer;

        switch (current_child->type)
        {
            case ElementType::Component:
                children_initializer = current_child->component->render();
                for (auto& child_initializer : children_initializer)
                {
                    render_child_element(current_child, child_initializer);
                }
                break;

            case ElementType::Root:
                // Log warning here.
                break;
            case ElementType::Window:
                // children_initializer = render_window(current_child, props);
                break;
            case ElementType::DrawCommand:
                // add_draw_command(current_child, props);
            default:
                break;
        }
    }

    // Append this element output to parent output.
}

void Application::render(const ComponentFactory* component_factory, core::Dict props)
{
    m_render_tree = std::make_shared<Element>();
    m_render_tree->type = ElementType::Root;
    m_render_tree->component = component_factory->create();
    
    m_render_tree->props = props;
    m_render_tree->state = m_render_tree->component->initialize_state(props);
    
    // Begin command buffer

    auto children_initializer = m_render_tree->component->render();
    for (auto& child_initializer : children_initializer)
    {
        render_child_element(m_render_tree, child_initializer);
    }

    // Flush command buffer
}

Application::~Application()
{
    assert(m_app_status == ApplicationStatus::Stopped);
    SDL_Quit();
}

} // namespace ui
