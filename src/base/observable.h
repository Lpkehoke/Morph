#pragma once

#include <functional>

namespace base
{

class Observable
{
  public:
      using OnUpdateFn = std::function<void()>;

      Observable();
      virtual ~Observable();

      void subscribe(OnUpdateFn on_update);

  protected:
      void notify();

  private:
      struct Impl;
      Impl* impl;
};

} // namespace base
