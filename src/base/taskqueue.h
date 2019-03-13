#pragma once

#include <functional>

namespace base
{

class TaskQueue
{
  public:
    using Task = std::function<void()>;

    TaskQueue();
    ~TaskQueue();

    void post(Task task);

  private:
    struct Impl;
    Impl* impl;
};

} // namespace base
