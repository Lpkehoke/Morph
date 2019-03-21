#pragma once

#include "computeenginetypes.h"

#include "platform/nodestoragetypes.h"

#include <tbb/task.h>

#include <functional>
#include <set>

namespace computeengine
{

class ComputeTask : public tbb::task
{
  public:
    using SetAttributeFn = std::function<void (platform::AttrId, platform::AttrPtr)>;
    using GetAttributeFn = std::function<platform::AttrPtr (platform::AttrId)>;
    using OnReadyFn = std::function<void()>;   

    ComputeTask(
        platform::NodeId    node_id,
        SetAttributeFn      set_attr,
        GetAttributeFn      get_attr,
        OnReadyFn           on_ready);

    virtual tbb::task* execute() override;

    bool add_successor(platform::NodeId successor);
    void bind_dependency_graph(DepsGraph deps_graph);
    void bind_node_storage_state(platform::NodeStorageState state);

  private:
    platform::NodeId            m_node_id;
    std::set<platform::NodeId>  m_successors;
    DepsGraph                   m_deps_graph;
    platform::NodeStorageState  m_state;
    SetAttributeFn              m_set_attr;
    GetAttributeFn              m_get_attr;
    OnReadyFn                   m_on_ready;
};

} // namespace computeengine