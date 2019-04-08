#pragma once

#include "base/observable.h"

#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include <functional>

namespace platform { class PluginManager; }
namespace platform { class Logger; }


namespace platform
{

class NodeStorage : public base::Observable
{
  public:
    using OnUpdateFn = std::function<void()>;

    NodeStorage(
        PluginManager*  plugin_manager,
        Logger*         logger);

    ~NodeStorage() override;

    void                dispatch(NodeStorageAction action);
    NodeStorageState    state() const;

  private:
    struct Impl;
    Impl* impl;
};

} // namespace platform
