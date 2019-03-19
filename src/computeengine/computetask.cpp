#include "computetask.h"

#include "platform/node.h"

#include <utility>

using namespace platform;


namespace computeengine
{

ComputeTask::ComputeTask(NodeId node_id)
    : m_node_id(node_id)
{}

tbb::task* ComputeTask::execute()
{
    auto& node = m_state.m_nodes[m_node_id];

    node->on_compute_begin(&m_state);
    node->compute();
    node->on_compute_end();

    for (auto& node_id : m_successors)
    {
        m_deps_graph[node_id]->decrement_ref_count();
    }

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
