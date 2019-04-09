#include "platform/nodestorage.h"

#include "platform/knob.h"
#include "platform/logger.h"
#include "platform/node.h"
#include "platform/pluginmanager.h"
#include "platform/nodestorageactions.h"
#include "platform/nodestoragetypes.h"
#include "platform/operations.h"

#include "base/immutable/map.h"
#include "base/taskqueue.h"
#include "base/observable.h"

#include <tbb/spin_mutex.h>

#include <exception>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

namespace
{

using namespace platform;

class Reducer
{
  public:
    Reducer(
        PluginManager*  plugin_manager,
        Logger*         logger)
        : m_plugin_manager(plugin_manager)
        , m_logger(logger)
    {}

    NodeStorageState reduce(NodeStorageState state, NodeStorageAction&& action)
    {
        return std::visit(
            [&](auto&& a)
            {
                return reduce(std::move(state), std::move(a));
            },
            std::move(action));
    }

    NodeStorageState reduce(NodeStorageState&& state, CreateNode&& action)
    {      
        auto node_id = state.m_next_node_id;
        auto next_state = make_node(std::move(state), *m_plugin_manager, action.model);

        next_state.m_node_metadata.mutable_set(node_id, std::move(action.metadata));

        return next_state;
    }

    NodeStorageState reduce(NodeStorageState&& state, RemoveNode&& action)
    {
        // TODO.
        return state;
    }

    NodeStorageState reduce(NodeStorageState&& state, UpdateNodeMetadata&& action)
    {
        auto next_state = std::move(state);

        auto metadata = next_state.m_node_metadata.get(action.id);

        for (auto pair : action.metadata)
        {
            metadata.mutable_set(pair.first, pair.second);
        }

        next_state.m_node_metadata.mutable_set(action.id, metadata);

        return next_state;
    }

    NodeStorageState reduce(NodeStorageState&& state, MakeConnection&& action)
    {
        auto next_state = std::move(state);

        NodePtr input_node = next_state.m_nodes.get(action.input_node_id);
        NodePtr output_node = next_state.m_nodes.get(action.output_node_id);

        KnobId input_knob_id = input_node->input_knobs().get(action.input_knob_name);
        KnobId output_knob_id = output_node->output_knobs().get(action.output_knob_name);

        KnobPtr input_knob = next_state.m_knobs.get(input_knob_id);
        KnobPtr output_knob = next_state.m_knobs.get(output_knob_id);

        if (!input_knob->accept(*output_knob))
        {
            throw std::runtime_error("Can't connect incompatible knobs.");
        }

        KnobPtr connected_input_knob = input_knob->connect(
            action.output_node_id,
            action.output_knob_name);

        next_state.m_knobs.mutable_set(
            input_knob_id,
            connected_input_knob);

        return next_state;
    }

    NodeStorageState reduce(NodeStorageState&& state, BreakConnection&& action)
    {
        auto next_state = std::move(state);
        auto& node = next_state.m_nodes[action.input_node_id];
        auto knob_id = node->input_knobs()[action.input_knob_name];
        auto& knob = next_state.m_knobs[knob_id];

        next_state.m_knobs.mutable_set(knob_id, knob->disconnect());
        return next_state;
    }

  private:
    PluginManager*  m_plugin_manager;
    Logger*         m_logger;
};

} // namespace

namespace platform
{

struct NodeStorage::Impl
{
    Impl(
        PluginManager*      plugin_manager,
        Logger*             logger)
        : m_reducer(plugin_manager, logger)
        , m_logger(logger)
    {}

    NodeStorageState        m_state;
    Reducer                 m_reducer;
    Logger*                 m_logger;
    base::TaskQueue         m_action_queue;
    tbb::spin_mutex         m_mutex_dispatch;
};

NodeStorage::NodeStorage(
    PluginManager*          plugin_manager,
    Logger*                 logger)
    : impl(new Impl(plugin_manager, logger))
{}

NodeStorage::~NodeStorage()
{
    delete impl;
}

void NodeStorage::dispatch(NodeStorageAction action)
{
    impl->m_action_queue.post([=]() mutable
    {
        tbb::spin_mutex::scoped_lock lock(impl->m_mutex_dispatch);

        try
        {
            impl->m_state = impl->m_reducer.reduce(
                impl->m_state,
                std::move(action));
        }
        catch (const std::exception& ex)
        {
            impl->m_logger->log(Logger::Severity::Error, ex.what());
        }

        impl->m_action_queue.post([=](){
            notify();
        });
    });

}

NodeStorageState NodeStorage::state() const
{
    // TODO: synchronization needed! (atomic root ptr in the map?)
    return impl->m_state;
}

} // namespace platform
