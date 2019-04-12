#pragma once

namespace pybind11 { class handle; }

void bind_dict(pybind11::handle scope);
void bind_logger(pybind11::handle scope);
void bind_node(pybind11::handle scope);
void bind_node_factory(pybind11::handle scope);
void bind_node_storage(pybind11::handle scope);
void bind_core(pybind11::handle scope);
void bind_plugin_manager(pybind11::handle scope);
