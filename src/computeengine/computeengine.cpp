#include "computeengine.h"

#include "computegraph.h"

#include "platform/nodestoragetypes.h"

#include "tbb/task.h"

#include <cstddef>
#include <map>
#include <stack>
#include <variant>

using namespace platform;

namespace computeengine
{

namespace
{

std::stack<NodeId> retrieve_observable_nodes(const NodeStorageState& state)
{
    std::stack<NodeId> result;

    for (const auto metadata_pair : state.m_node_metadata)
    {
        const auto& metadata = metadata_pair.second;

        if (metadata.contains("compute_engine_observable")
            && std::get<bool>(metadata.get("compute_engine_observable")))
        {
            result.push(metadata_pair.first);
        }
    }

    return result;
}

} // namespace


ComputeEngine::ComputeEngine(const platform::EnableDispatch& enable_dispatch)
    : platform::EnableDispatch(enable_dispatch)
{}

void ComputeEngine::on_update() const
{
    NodeStorageState state = fetch_state();

    std::stack<NodeId> nodes_to_process = retrieve_observable_nodes(state);

    ComputeGraph compute_graph(*this);

    while (!nodes_to_process.empty())
    {
        auto node_id = nodes_to_process.top();
        nodes_to_process.pop();

        compute_graph.add_node(node_id);

        const auto& node = state.m_nodes.get(node_id);

        for (const auto& input_knob_pair : node->input_knobs())
        {
            KnobId knob_id = input_knob_pair.second;

            const auto& knob = state.m_knobs.get(knob_id);

            if (knob->is_reference())
            {
                auto referenced_node_id = knob->referenced_node();
                nodes_to_process.push(referenced_node_id);
                compute_graph.add_node(referenced_node_id);
                compute_graph.add_connection(referenced_node_id, node_id);
            }
        }
    }

    compute_graph.spawn_compute_tasks(state);
}

} // namespace computeengine
