#pragma once

#include "platform/nodestoragetypes.h"

#include "base/immutable/map.h"

#include <tbb/task.h>

#include <set>

namespace computeengine
{

class ComputeTask : public tbb::task
{
  public:
    using DepsGraph = immutable::Map<platform::NodeId, ComputeTask*>;

    ComputeTask(platform::NodeId node_id);

    virtual tbb::task* execute() override;

    bool add_successor(platform::NodeId successor);
    void bind_dependency_graph(DepsGraph deps_graph);
    void bind_node_storage_state(platform::NodeStorageState state);

  private:
    platform::NodeId            m_node_id;
    std::set<platform::NodeId>  m_successors;
    DepsGraph                   m_deps_graph;
    platform::NodeStorageState  m_state;   
};

} // namespace computeengine