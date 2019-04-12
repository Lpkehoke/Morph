#pragma once

#include "core/nodestoragetypes.h"

namespace core { class KnobSchema; }
namespace core { class PluginManager; }

namespace core
{

NodeStorageState make_knob(
    NodeStorageState        state,
    NodeId                  owner,
    const KnobSchema&       knob_schema);

NodeStorageState make_node(
    NodeStorageState        state,
    const PluginManager&    plugin_manager,
    const std::string&      node_model);

} // namespace core
