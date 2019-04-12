#pragma once

#include "core/nodestorageactions.h"
#include "core/nodestoragetypes.h"

#include <pybind11/pybind11.h>

template <typename T>
T from_dict(const pybind11::dict& input);

#ifndef PYTHON_DICT_CONVERSION_IMPL

extern template core::Metadata from_dict(const pybind11::dict& input);
extern template core::CreateNode from_dict(const pybind11::dict& input);
extern template core::RemoveNode from_dict(const pybind11::dict& input);
extern template core::UpdateNodeMetadata from_dict(const pybind11::dict& input);
extern template core::MakeConnection from_dict(const pybind11::dict& input);

#endif
