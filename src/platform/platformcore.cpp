#include "platform/platformcore.h"

#include <memory>

namespace platform
{

PlatformCore::PlatformCore()
{
    m_logger = std::make_shared<Logger>();
    m_plugin_manager = std::make_shared<PluginManager>();

    m_node_storage = std::make_shared<NodeStorage>(m_plugin_manager, m_logger);
}

} // namespace platform
