#pragma once

#include "computeenginetypes.h"

#include "core/enabledispatch.h"
#include "core/nodestoragetypes.h"

#include <tbb/concurrent_unordered_map.h>

#include <atomic>


namespace computeengine
{

class ComputeGraph : public core::EnableDispatch
{
  public:
    ComputeGraph(
        const core::EnableDispatch& enable_dispatch);

    void add_node(core::NodeId node_id);
    void add_connection(
        core::NodeId output_node_id,
        core::NodeId input_node_id);
    
    void spawn_compute_tasks(core::NodeStorageState state);

  private:
    using AttrMap = tbb::concurrent_unordered_map<
        core::AttrId,
        core::AttrPtr>;

    void finalize();

    DepsGraph                       m_deps_graph;
    AttrMap                         m_attr_map;
    std::atomic_int32_t             m_ref_count;
};

} // namespace computeengine
