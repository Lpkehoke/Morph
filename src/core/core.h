#pragma once

#include "core/logger.h"
#include "core/nodestorage.h"
#include "core/nodestorageactions.h"
#include "core/coretypes.h"
#include "core/pluginmanager.h"

#include <memory>


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
