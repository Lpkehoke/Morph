#pragma once

#include "platform/nodestoragetypes.h"

namespace platform { class KnobSchema; }
namespace platform { class PluginManager; }

namespace platform
{

NodeStorageState make_knob(
    NodeStorageState        state,
    NodeId                  owner,
    const KnobSchema&       knob_schema);

NodeStorageState make_node(
    NodeStorageState        state,
    const PluginManager&    plugin_manager,
    const std::string&      node_model);

} // namespace platform
