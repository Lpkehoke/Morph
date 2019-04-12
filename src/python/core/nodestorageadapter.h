#pragma once

#include "core/nodestorage.h"
#include "core/coretypes.h"

#include <pybind11/pybind11.h>

namespace core { class NodeStorageState; }


class NodeStorageAdapter
{
  public:
    NodeStorageAdapter(core::NodeStoragePtr node_storage);

    void dispatch(const pybind11::dict& action);
    void subscribe(const pybind11::object& on_update);
    core::NodeStorageState state();
  
  private:
    core::NodeStoragePtr m_node_storage;
};
