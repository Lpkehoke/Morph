#include "computetaskdagbuilder.h"

#include <tbb/task.h>

#include <exception>

namespace computeengine
{

void ComputeTaskDAGBuilder::add_node(platform::NodeId node_id)
{
    if (!m_deps_graph.contains(node_id))
    {
        m_deps_graph.mutable_set(
            node_id,
            new (tbb::task::allocate_root()) ComputeTask(node_id));
    }
}

void ComputeTaskDAGBuilder::add_connection(
    platform::NodeId output_node_id,
    platform::NodeId input_node_id)
{
    if (!m_deps_graph.contains(output_node_id))
    {
        throw std::runtime_error("Unknown id specified for output node id.");
    }
    if (!m_deps_graph.contains(input_node_id))
    {
        throw std::runtime_error("Unknown id specified for input node id.");
    }

    auto& output_node = m_deps_graph.mutable_get(output_node_id);
    auto& input_node = m_deps_graph.mutable_get(input_node_id);

    if (output_node->add_successor(input_node_id))
    {
        input_node->increment_ref_count();
    }
}
    
void ComputeTaskDAGBuilder::spawn_compute_tasks(platform::NodeStorageState state)
{
    tbb::task_list list;
    for (auto task : m_deps_graph)
    {
        task.second->bind_dependency_graph(m_deps_graph);
        task.second->bind_node_storage_state(state);
        list.push_back(*(task.second));
    }

    tbb::task::spawn(list);
}

} // namespace computeengine
