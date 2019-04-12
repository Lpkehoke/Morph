#pragma once

#include "core/coretypes.h"

namespace core
{

class Core
{
  public:
    Core();

    NodeStoragePtr node_storage() const;
    PluginManagerPtr plugin_manager() const;
    LoggerPtr logger() const;

  private:
    NodeStoragePtr      m_node_storage;
    PluginManagerPtr    m_plugin_manager;
    LoggerPtr           m_logger;
};

} // namespace core
