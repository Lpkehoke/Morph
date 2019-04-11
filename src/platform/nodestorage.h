#pragma once

#include "base/observable.h"

#include "nodestorageactions.h"
#include "nodestoragetypes.h"

#include <functional>

namespace platform { class NodeFactoryRegistry; }
namespace platform { class Logger; }


namespace platform
{

class NodeStorage : public base::Observable
{
  public:
    using OnUpdateFn = std::function<void()>;

    NodeStorage(
        NodeFactoryRegistry*    node_factory_registry,
        Logger*                 logger);

    ~NodeStorage() override;

    void                dispatch(NodeStorageAction action);
    NodeStorageState    state() const;

  private:
    struct Impl;
    Impl* impl;
};

} // namespace platform
