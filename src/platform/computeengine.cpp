#include "computeengine.h"

namespace platform
{

ComputeEngine::ComputeEngine(const EnableDispatch& enable_dispatch)
    : EnableDispatch(enable_dispatch)
{}

void ComputeEngine::on_update(NodeCollection node_storage_state) const
{}

void ComputeEngine::compute(Node*           node,
                            NodeCollection  node_storage_state) const
{}

} // namespace platform
