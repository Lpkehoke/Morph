#include "observable.h"

#include <tbb/spin_mutex.h>

#include <exception>
#include <utility>
#include <vector>

namespace foundation
{
    struct Observable::Impl
    {
        std::vector<OnUpdateFn>    m_subscribers;
        tbb::spin_mutex            m_mutex;
    };

    Observable::Observable()
        : impl(new Impl())
    {}

    Observable::~Observable()
    {
        delete impl;
    }

    void Observable::subscribe(OnUpdateFn on_update)
    {
        tbb::spin_mutex::scoped_lock lock(impl->m_mutex);
        impl->m_subscribers.emplace_back(std::move(on_update));
    }

    void Observable::notify() const
    {
        tbb::spin_mutex::scoped_lock lock(impl->m_mutex);

        for (const auto& cb : impl->m_subscribers)
        {
            try
            {
                cb();
            }
            catch(const std::exception& ex)
            {
                ex.what();
            }
        }
    }

} // namespace foundation
