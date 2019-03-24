#pragma once

#include "computeenginetypes.h"

#include "platform/enabledispatch.h"
#include "platform/nodestoragetypes.h"

#include <tbb/concurrent_unordered_map.h>

#include <atomic>


namespace computeengine
{

class ComputeGraph : public platform::EnableDispatch
{
  public:
    ComputeGraph(
        const platform::EnableDispatch& enable_dispatch);

    void add_node(platform::NodeId node_id);
    void add_connection(
        platform::NodeId output_node_id,
        platform::NodeId input_node_id);
    
    void spawn_compute_tasks(platform::NodeStorageState state);

  private:
    using AttrMap = tbb::concurrent_unordered_map<
        platform::AttrId,
        platform::AttrPtr>;

    void finalize();

    DepsGraph                       m_deps_graph;
    AttrMap                         m_attr_map;
    std::atomic_int32_t             m_ref_count;
};

} // namespace computeengine
