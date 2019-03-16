#include "nodestorage.h"

#include "attribute.h"
#include "knobsbuilder.h"
#include "logger.h"
#include "node.h"
#include "nodefactory.h"
#include "nodefactoryregistry.h"
#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include "base/taskqueue.h"

#include <exception>
#include <memory>
#include <vector>

namespace
{

using namespace platform;

class Reducer
{
  public:
    Reducer(
        NodeFactoryRegistry*    registry,
        KnobsBuilder*           builder,
        Logger*                 logger)
        : m_node_factory_registry(registry)
        , m_knobs_builder(builder)
        , m_logger(logger)
        , m_next_attr_id(0u)
        , m_next_knob_id(0u)
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
        auto next_state = std::move(state);

        auto factory = m_node_factory_registry->get_node_factory(action.model);

        NodeFactory::KnobsMap knobs_map;

        auto knobs_info = factory->knobs_init_info();
        for (const auto& knob_info : knobs_info)
        {
            auto& knob_name = knob_info.first;
            auto& knob_model = knob_info.second.model;
            auto& default_values = knob_info.second.default_values;

            Knob::AttrMap knob_attr_map;
            for (const auto& attr_pair : default_values)
            {
                AttrId attr_id = m_next_attr_id++;

                AttrPtr attr = std::make_shared<Attribute>(*attr_pair.second);
                
                next_state.m_attributes.mutable_set(attr_id, attr);
                knob_attr_map.mutable_set(attr_pair.first, attr_id);
            }

            KnobPtr knob(m_knobs_builder->build(knob_model, knob_attr_map));
            KnobId knob_id = m_next_knob_id++;

            // TODO: populate knob metadata.
            next_state.m_knobs.mutable_set(knob_id, std::move(knob));

            knobs_map.emplace(knob_name, knob_id);
        }

        NodePtr new_node(factory->create(knobs_map));

        next_state.m_nodes.mutable_set(action.id, std::move(new_node));
        next_state.m_node_metadata.mutable_set(action.id, std::move(action.metadata));

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

  private:
    NodeFactoryRegistry*    m_node_factory_registry;
    KnobsBuilder*           m_knobs_builder;
    Logger*                 m_logger;
    AttrId                  m_next_attr_id;
    KnobId                  m_next_knob_id;
};

} // namespace

namespace platform
{

struct NodeStorage::Impl
{
    Impl(NodeFactoryRegistry* registry, KnobsBuilder* builder, Logger* logger)
        : m_reducer(registry, builder, logger)
        , m_logger(logger)
    {}

    base::TaskQueue         m_action_queue;
    NodeStorageState        m_state;
    Reducer                 m_reducer;
    std::vector<OnUpdateFn> m_subscribers;
    Logger*                 m_logger;
};

NodeStorage::NodeStorage(
    NodeFactoryRegistry*    registry,
    KnobsBuilder*           builder,
    Logger*                 logger)
    : impl(new Impl(registry, builder, logger))
{}

NodeStorage::~NodeStorage()
{
    delete impl;
}

void NodeStorage::dispatch(NodeStorageAction action)
{
    impl->m_action_queue.post([=]() mutable
    {
        try
        {
            impl->m_state = impl->m_reducer.reduce(
                std::move(impl->m_state),
                std::move(action));
            
            for (auto& cb : impl->m_subscribers)
            {
                try
                {
                    cb();
                }
                catch(const std::exception& ex)
                {
                    ex.what();
                }
            }
        }
        catch (const std::exception& ex)
        {
            impl->m_logger->log_error(ex.what());
        }
    });
}

NodeStorageState NodeStorage::state() const
{
    // TODO: synchronization needed! (atomic root ptr in the map?)
    return impl->m_state;
}

void NodeStorage::subscribe(OnUpdateFn on_update)
{
    impl->m_subscribers.push_back(std::move(on_update));
}

} // namespace platform
