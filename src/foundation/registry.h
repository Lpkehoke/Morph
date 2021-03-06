#pragma once

#include "foundation/format.h"

#include <functional>
#include <map>
#include <string>

namespace foundation
{

template <typename Entity>
class Registry
{
  public:
    using EntityMap = std::map<std::string, Entity>;
    using ConstIterator = typename EntityMap::const_iterator;
    using GetEntityName = std::function<std::string (const Entity&)>;
    
    Registry(GetEntityName get_entity_name);

    void register_entity(Entity entity);
    const Entity& get(const std::string& name) const;

    ConstIterator begin() const;
    ConstIterator end() const;
  
  private:
    GetEntityName   m_get_entity_name;
    EntityMap       m_entity_map;
};


//
//  Registry implementation.
//

template <typename Entity>
Registry<Entity>::Registry(GetEntityName get_entity_name)
  : m_get_entity_name(std::move(get_entity_name))
{}

template <typename Entity>
void Registry<Entity>::register_entity(Entity entity)
{
    std::string name = m_get_entity_name(entity);

    auto itr = m_entity_map.find(name);
    if (itr != m_entity_map.end())
    {
        throw std::runtime_error(format("Entity with name \'%s\' already exists.", name.c_str()));
    }

    m_entity_map.emplace(name, std::move(entity));
}

template <typename Entity>
const Entity& Registry<Entity>::get(const std::string& name) const
{
    auto itr = m_entity_map.find(name);
    if (itr == m_entity_map.end())
    {
        throw std::runtime_error(format("Entity with name \'%s\' does not exists.", name.c_str()));
    }

    return itr->second;
}

template <typename Entity>
typename Registry<Entity>::ConstIterator Registry<Entity>::begin() const
{
    return m_entity_map.begin();
}

template <typename Entity>
typename Registry<Entity>::ConstIterator Registry<Entity>::end() const
{
    return m_entity_map.end();
}

} // namespace foundation
