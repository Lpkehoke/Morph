#pragma once

#include "computetask.h"

#include "platform/nodestoragetypes.h"

#include <tuple>
#include <list>
#include <set>

namespace computeengine
{

class ComputeTaskDAGBuilder
{
  public:
    void add_node(platform::NodeId node_id);
    void add_connection(
        platform::NodeId output_node_id,
        platform::NodeId input_node_id);
    
    void spawn_compute_tasks(platform::NodeStorageState state);

  private:    
    ComputeTask::DepsGraph m_deps_graph;
};

} // namespace computeengine
