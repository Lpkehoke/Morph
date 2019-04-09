#pragma once

#include <memory>

namespace platform { class NodeStorage; }
namespace platform { class PluginManager; }
namespace platform { class Logger; }


namespace platform
{

using NodeStoragePtr = std::shared_ptr<NodeStorage>;
using PluginManagerPtr = std::shared_ptr<PluginManager>;
using LoggerPtr = std::shared_ptr<Logger>;

} // namespace platform
