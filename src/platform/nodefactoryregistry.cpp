#include "nodefactoryregistry.h"

#include "nodefactory.h"

#include <exception>
#include <utility>

namespace platform
{

void NodeFactoryRegistry::register_node_factory(NodeFactory* factory)
{
    auto model = factory->model();

    auto itr = m_factory_collection.find(model);
    if (itr != m_factory_collection.end())
    {
        throw std::runtime_error("Node factory for model " + model + " already exists.");
    }

    m_factory_collection.emplace(model, factory);
}

const NodeFactory* NodeFactoryRegistry::get_node_factory(
    const std::string& model) const
{
    auto itr = m_factory_collection.find(model);
    if (itr == m_factory_collection.end())
    {
        throw std::runtime_error("Node factory for model " + model + " not found.");
    }

    return itr->second;
}

} // namespace platform
