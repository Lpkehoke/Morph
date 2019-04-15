#pragma once

#include "core/nodestoragetypes.h"

#include <string>

namespace core { class KnobSchema; }
namespace core { class PluginManager; }
namespace foundation { class Dict; }

namespace core
{

NodeStorageState make_knob(
    NodeStorageState        state,
    NodeId                  owner,
    const KnobSchema&       knob_schema);

NodeStorageState make_node(
    NodeStorageState        state,
    const PluginManager&    plugin_manager,
    const std::string&      node_model,
    Dict                    node_metadata);

NodeStorageState remove_knob(
    NodeStorageState        state,
    KnobId                  knob_id);

NodeStorageState remove_node(
    NodeStorageState        state,
    NodeId                  node_id);

} // namespace core
