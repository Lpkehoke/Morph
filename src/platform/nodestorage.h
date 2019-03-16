#pragma once

#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include "base/immutable/map.h"

#include <functional>

namespace platform { class KnobsBuilder; }
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
        KnobsBuilder*           builder,
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
