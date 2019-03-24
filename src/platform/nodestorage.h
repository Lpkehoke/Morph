#pragma once

#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include <functional>

namespace platform { class KnobModelRegistry; }
namespace platform { class Logger; }
namespace platform { class NodeFactoryRegistry; }

namespace platform
{

class NodeStorage
{
  public:
    using OnUpdateFn = std::function<void()>;

    NodeStorage(
        NodeFactoryRegistry*    registry,
        KnobModelRegistry*     builder,
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
