#include "foundation/taskqueue.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

using namespace foundation;
using namespace testing;
using namespace std::chrono_literals;


class AsyncCallback {
public:
    virtual void on_data(int seed) = 0;

    virtual ~AsyncCallback() {}
};

class MockCallback : public AsyncCallback {
public:
    MOCK_METHOD1(on_data, void(int));
};


TEST(TaskQueue, post_task)
{
    TaskQueue queue;
    auto cb_mock = std::make_shared<StrictMock<MockCallback>>();
    
    std::atomic<int> previous_seed(0);
    std::atomic<bool> ready(false);
    std::mutex mutex;
    std::condition_variable cv;

    for (auto seed = 1; seed < 10000; ++seed)
    {
        EXPECT_CALL((*cb_mock.get()), on_data(seed)).Times(1);
    }

    for (auto seed = 1; seed < 10000; ++seed)
    {
        queue.post([seed, cb_mock, &previous_seed]()
            {
                cb_mock->on_data(seed);
                ASSERT_EQ(previous_seed, seed - 1);
                std::this_thread::sleep_for(5ns);
                previous_seed.store(seed);
            });
    }

    queue.post([&]()
    {
        std::unique_lock<std::mutex> lock(mutex);
        ready.store(true);
        lock.unlock();
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(
        lock,
        [&ready]
        {
            return ready.load();
        });
}
