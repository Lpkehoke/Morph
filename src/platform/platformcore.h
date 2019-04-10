#pragma once

#include "platform/logger.h"
#include "platform/nodestorage.h"
#include "platform/nodestorageactions.h"
#include "platform/platformcoretypes.h"
#include "platform/pluginmanager.h"

#include <memory>


namespace platform
{

class PlatformCore
{
  public:
    PlatformCore();

    NodeStoragePtr node_storage() const;
    PluginManagerPtr plugin_manager() const;
    LoggerPtr logger() const;

  private:
    NodeStoragePtr      m_node_storage;
    PluginManagerPtr    m_plugin_manager;
    LoggerPtr           m_logger;
};

} // namespace platform
