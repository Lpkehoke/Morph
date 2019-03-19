#include "enabledispatch.h"

namespace platform
{

EnableDispatch::EnableDispatch(const DispatchFn& dispatch, const StateFn& state)
    : m_dispatch(dispatch)
    , m_state(state)
{}

EnableDispatch::EnableDispatch(const EnableDispatch& other)
    : m_dispatch(other.m_dispatch)
    , m_state(other.m_state)
{}

void EnableDispatch::dispatch(NodeStorageAction action) const
{
    if (m_dispatch)
    {
        m_dispatch(std::move(action));
    }
}

NodeStorageState EnableDispatch::fetch_state() const
{
    if (m_state)
    {
        return m_state();
    }

    return NodeStorageState();
}

} // namespace platform
