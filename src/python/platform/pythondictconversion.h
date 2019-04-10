#pragma once

#include "platform/nodestorageactions.h"
#include "platform/nodestoragetypes.h"

#include <pybind11/pybind11.h>

template <typename T>
T from_dict(const pybind11::dict& input);

#ifndef PYTHON_DICT_CONVERSION_IMPL

extern template platform::Metadata from_dict(const pybind11::dict& input);
extern template platform::CreateNode from_dict(const pybind11::dict& input);
extern template platform::RemoveNode from_dict(const pybind11::dict& input);
extern template platform::UpdateNodeMetadata from_dict(const pybind11::dict& input);
extern template platform::MakeConnection from_dict(const pybind11::dict& input);

#endif
