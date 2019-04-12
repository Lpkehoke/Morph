#pragma once

#include <memory>

namespace core { class NodeStorage; }
namespace core { class PluginManager; }
namespace core { class Logger; }


namespace core
{

using NodeStoragePtr = std::shared_ptr<NodeStorage>;
using PluginManagerPtr = std::shared_ptr<PluginManager>;
using LoggerPtr = std::shared_ptr<Logger>;

} // namespace core
