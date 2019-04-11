#pragma once

#include "platform/nodestorage.h"
#include "platform/platformcoretypes.h"

#include <pybind11/pybind11.h>

namespace platform { class NodeStorageState; }


class NodeStorageAdapter
{
  public:
    NodeStorageAdapter(platform::NodeStoragePtr node_storage);

    void dispatch(const pybind11::dict& action);
    void subscribe(const pybind11::object& on_update);
    platform::NodeStorageState state();
  
  private:
    platform::NodeStoragePtr m_node_storage;
};
