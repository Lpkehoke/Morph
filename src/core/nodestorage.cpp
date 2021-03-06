#include "core/nodestorage.h"

#include "core/coretypes.h"
#include "core/dict.h"
#include "core/knob.h"
#include "core/logger.h"
#include "core/node.h"
#include "core/nodestorageactions.h"
#include "core/nodestoragetypes.h"
#include "core/operations.h"

#include "foundation/immutable/map.h"
#include "foundation/taskqueue.h"

#include <tbb/spin_mutex.h>

#include <exception>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

using namespace foundation;


namespace core
{

namespace
{

class Reducer
{
  public:
    Reducer(
        PluginManagerPtr   plugin_manager,
        LoggerPtr          logger)
        : m_plugin_manager(std::move(plugin_manager))
        , m_logger(std::move(logger))
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
        auto next_state = make_node(
            std::move(state),
            *m_plugin_manager,
            action.model,
            std::move(action.metadata));

        return next_state;
    }

    NodeStorageState reduce(NodeStorageState&& state, RemoveNode&& action)
    {
        auto next_state = remove_node(std::move(state), action.id);
        return next_state;
    }

    NodeStorageState reduce(NodeStorageState&& state, UpdateNodeMetadata&& action)
    {
        auto next_state = std::move(state);

        next_state.m_node_metadata.mutable_get(action.id).update(action.metadata);

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
    PluginManagerPtr   m_plugin_manager;
    LoggerPtr          m_logger;
};

} // namespace

struct NodeStorage::Impl
{
    Impl(
        PluginManagerPtr    plugin_manager,
        LoggerPtr           logger)
        : m_reducer(std::move(plugin_manager), logger)
        , m_logger(std::move(logger))
        , m_action_queue(TaskQueue::Priority::High)
    {}

    NodeStorageState        m_state;
    Reducer                 m_reducer;
    LoggerPtr               m_logger;
    TaskQueue               m_action_queue;
    tbb::spin_mutex         m_mutex_dispatch;
};

NodeStorage::NodeStorage(
    PluginManagerPtr        plugin_manager,
    LoggerPtr               logger)
    : impl(new Impl(
        std::move(plugin_manager),
        std::move(logger)))
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

        lock.release();
        notify();
    });

}

NodeStorageState NodeStorage::state() const
{
    // TODO: synchronization needed! (atomic root ptr in the map?)
    return impl->m_state;
}

} // namespace core
