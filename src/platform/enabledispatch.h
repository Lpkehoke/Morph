#pragma once

#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include <functional>

namespace platform
{

class EnableDispatch
{
  public:
    using DispatchFn = std::function<void(NodeStorageAction)>;
    using StateFn    = std::function<NodeStorageState()>;

    EnableDispatch(const DispatchFn& dispatch, const StateFn& state);
    EnableDispatch(const EnableDispatch& other);

  protected:
    void dispatch(NodeStorageAction action) const;
    NodeStorageState fetch_state() const;

  private:
    DispatchFn m_dispatch;
    StateFn    m_state;
};

} // namespace platform
