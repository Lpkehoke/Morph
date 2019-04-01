#pragma once

#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include <functional>

namespace platform { class NodeFactoryRegistry; }
namespace platform { class Logger; }


namespace platform
{

class NodeStorage
{
  public:
    using OnUpdateFn = std::function<void()>;

    NodeStorage(
        NodeFactoryRegistry*    node_factory_registry,
        Logger*                 logger);

    ~NodeStorage();

    void                dispatch(NodeStorageAction action);
    NodeStorageState    state() const;
    void                subscribe(OnUpdateFn on_update);

  private:
    struct Impl;
    Impl* impl;
};

} // namespace platform
