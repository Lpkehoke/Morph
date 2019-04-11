#include "nodestorage.h"

#include "knob.h"
#include "logger.h"
#include "node.h"
#include "nodefactory.h"
#include "nodefactoryregistry.h"
#include "nodestorageactions.h"
#include "nodestoragetypes.h"
#include "value.h"

#include "base/immutable/map.h"
#include "base/taskqueue.h"
#include "base/observable.h"

#include <tbb/spin_mutex.h>

#include <memory>
#include <stdexcept>
#include <string>
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
        NodeFactoryRegistry*    node_factoryregistry,
        Logger*                 logger)
        : m_node_factory_registry(node_factoryregistry)
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
        auto factory = m_node_factory_registry->get_node_factory(action.model);

        auto node_id = state.m_next_node_id;
        auto next_state = factory->create(std::move(state));

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
    NodeFactoryRegistry*    m_node_factory_registry;
    Logger*                 m_logger;
};

} // namespace

namespace platform
{

struct NodeStorage::Impl
{
    Impl(
        NodeFactoryRegistry*    node_factory_registry,
        Logger*                 logger)
        : m_reducer(node_factory_registry, logger)
        , m_logger(logger)
    {}

    NodeStorageState        m_state;
    Reducer                 m_reducer;
    Logger*                 m_logger;
    base::TaskQueue         m_action_queue;
    tbb::spin_mutex         m_mutex_dispatch;
};

NodeStorage::NodeStorage(
    NodeFactoryRegistry*    node_factory_registry,
    Logger*                 logger)
    : impl(new Impl(node_factory_registry, logger))
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
                std::move(impl->m_state),
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
