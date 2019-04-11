#pragma once

#include <functional>

namespace base
{

class Observable
{
  public:
      using OnUpdateFn = std::function<void()>;

      Observable();
      ~Observable();

      void subscribe(OnUpdateFn on_update);

  protected:
      void notify() const;

  private:
      struct Impl;
      Impl* impl;
};

} // namespace base
