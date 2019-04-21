#pragma once

namespace core { class Dict; }
namespace core { class CreateNode; }
namespace core { class RemoveNode; }
namespace core { class UpdateNodeMetadata; }
namespace core { class MakeConnection; }

namespace pybind11 { class dict; }

template <typename T>
T from_python_dict(const pybind11::dict& input);

#ifndef PYTHON_DICT_CONVERSION_IMPL

extern template core::Dict from_python_dict(const pybind11::dict& input);
extern template core::CreateNode from_python_dict(const pybind11::dict& input);
extern template core::RemoveNode from_python_dict(const pybind11::dict& input);
extern template core::UpdateNodeMetadata from_python_dict(const pybind11::dict& input);
extern template core::MakeConnection from_python_dict(const pybind11::dict& input);

#endif
