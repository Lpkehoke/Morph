#include "computegraph.h"

#include "computetask.h"

#include "platform/nodestorageactions.h"

#include <tbb/task.h>

#include <exception>
#include <utility>

using namespace platform;

namespace computeengine
{

ComputeGraph::ComputeGraph(
    const EnableDispatch& enable_dispatch)
    : EnableDispatch(enable_dispatch)
    , m_ref_count(0)
{}

void ComputeGraph::add_node(NodeId node_id)
{
    if (!m_deps_graph.contains(node_id))
    {
        auto set_attr = [=](AttrId attr_id, AttrPtr attr_value)
        {
            m_attr_map.emplace(attr_id, attr_value);
        };

        auto get_attr = [=](AttrId attr_id)
        {
            const auto itr = m_attr_map.find(attr_id);
            
            if (itr == m_attr_map.end())
            {
                return AttrPtr {};
            }

            return itr->second;
        };

        auto on_ready = [=]()
        {
            --m_ref_count;
            if (m_ref_count <= 0)
            {
                finalize();
            }
        };

        m_deps_graph.mutable_set(
            node_id,
            new (tbb::task::allocate_root()) ComputeTask(
                node_id,
                std::move(set_attr),
                std::move(get_attr),
                std::move(on_ready)));
        
        ++m_ref_count;
    }
}

void ComputeGraph::add_connection(
    NodeId output_node_id,
    NodeId input_node_id)
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
    
void ComputeGraph::spawn_compute_tasks(NodeStorageState state)
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

void ComputeGraph::finalize()
{
    for (const auto& attr_pair : m_attr_map)
    {
        auto& attr_id = attr_pair.first;
        auto& attr_value = attr_pair.second;

        dispatch(UpdateAttribute {attr_id, attr_value});
    }
}

} // namespace computeengine
