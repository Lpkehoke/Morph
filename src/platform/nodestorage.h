#pragma once

#include "nodestorageactions.h"
#include "nodestoragetypes.h"
#include "platform/platformcoretypes.h"

#include "base/observable.h"

#include <functional>


namespace platform
{

class NodeStorage : public base::Observable
{
  public:
    using OnUpdateFn = std::function<void()>;

    NodeStorage(
        PluginManagerPtr   plugin_manager,
        LoggerPtr          logger);

    ~NodeStorage() override;

    void                dispatch(NodeStorageAction action);
    NodeStorageState    state() const;

  private:
    struct Impl;
    Impl* impl;
};

} // namespace platform
