#pragma once

#include "nodestorageactions.h"
#include "nodestoragetypes.h"
#include "core/coretypes.h"

#include "foundation/observable.h"

#include <functional>


namespace core
{

class NodeStorage : public foundation::Observable
{
  public:
    using OnUpdateFn = std::function<void()>;

    NodeStorage(
        PluginManagerPtr   plugin_manager,
        LoggerPtr          logger);

    ~NodeStorage();

    void                dispatch(NodeStorageAction action);
    NodeStorageState    state() const;

  private:
    struct Impl;
    Impl* impl;
};

} // namespace core
