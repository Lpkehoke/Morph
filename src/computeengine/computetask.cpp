#include "computetask.h"

#include "platform/knob.h"
#include "platform/node.h"

#include "base/immutable/map.h"

#include "tbb/task.h"

#include <utility>

using namespace platform;


namespace computeengine
{

ComputeTask::ComputeTask(
    NodeId          node_id,
    SetAttributeFn  set_attr,
    GetAttributeFn  get_attr,
    OnReadyFn       on_ready)
    : m_node_id(node_id)
    , m_set_attr(std::move(set_attr))
    , m_get_attr(std::move(get_attr))
    , m_on_ready(std::move(on_ready))
{}

tbb::task* ComputeTask::execute()
{
    auto& node = m_state.m_nodes[m_node_id];

    auto input_knobs = node->input_knobs();
    for (const auto& knob_pair : input_knobs)
    {
        auto& knob_id = knob_pair.second;

        auto& knob = m_state.m_knobs[knob_id];
        for (const auto& attribute_pair : *knob)
        {
            auto& attr_id = attribute_pair.second;
            
            auto updated_value = m_get_attr(attr_id);
            if (!updated_value)
            {
                continue;
            }

            m_state.m_attributes.mutable_set(attr_id, updated_value);
        }
    }

    node->on_compute_begin(&m_state);
    node->compute();
    node->on_compute_end();

    if (is_cancelled())
    {
        return nullptr;
    }

    auto output_knobs = node->output_knobs();
    for (const auto& knob_pair : output_knobs)
    {
        auto& knob_id = knob_pair.second;

        auto& knob = m_state.m_knobs[knob_id];
        for (const auto& attribute_pair : *knob)
        {
            auto& attr_id = attribute_pair.second;
            auto& attr_value = m_state.m_attributes[attr_id];

            m_set_attr(attr_id, attr_value);
        }
    }

    for (auto& node_id : m_successors)
    {
        m_deps_graph[node_id]->decrement_ref_count();
    }

    m_on_ready();

    return nullptr;
}

bool ComputeTask::add_successor(NodeId successor)
{
    auto res = m_successors.insert(successor);
    return res.second;
}

void ComputeTask::bind_dependency_graph(DepsGraph deps_graph)
{
    m_deps_graph = std::move(deps_graph);
}

void ComputeTask::bind_node_storage_state(NodeStorageState state)
{
    m_state = std::move(state);
}

} // namespace computeengine
