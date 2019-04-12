#pragma once

#include "computeenginetypes.h"

#include "core/nodestoragetypes.h"

#include <tbb/task.h>

#include <functional>
#include <set>

namespace computeengine
{

class ComputeTask : public tbb::task
{
  public:
    using SetAttributeFn = std::function<void (core::AttrId, core::AttrPtr)>;
    using GetAttributeFn = std::function<core::AttrPtr (core::AttrId)>;
    using OnReadyFn = std::function<void()>;   

    ComputeTask(
        core::NodeId        node_id,
        SetAttributeFn      set_attr,
        GetAttributeFn      get_attr,
        OnReadyFn           on_ready);

    virtual tbb::task* execute() override;

    bool add_successor(core::NodeId successor);
    void bind_dependency_graph(DepsGraph deps_graph);
    void bind_node_storage_state(core::NodeStorageState state);

  private:
    core::NodeId                m_node_id;
    std::set<core::NodeId>      m_successors;
    DepsGraph                   m_deps_graph;
    core::NodeStorageState      m_state;
    SetAttributeFn              m_set_attr;
    GetAttributeFn              m_get_attr;
    OnReadyFn                   m_on_ready;
};

} // namespace computeengine