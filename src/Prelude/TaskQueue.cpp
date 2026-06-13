module;
#include <BS_thread_pool.hpp>
#include <functional>
#include <mutex>
#include <vector>
#include <windalive.hpp>
export module Prelude.TaskQueue;

namespace Prelude {
export class TaskQueue
{
private:
  BS::thread_pool<BS::tp::none> pool;
  std::vector<std::function<void()>> tasks;
  std::mutex mtx;

public:
  TaskQueue()
    : pool(1)
  {
    tasks.reserve(100);
  }

  TaskQueue(const TaskQueue& _) = delete;

  template<class Function>
  fn enqueue(Function&& func) -> void
  {
    // TODO: make 2 vectors of tasks
    //       switch atomic pointer on wait
    pool.detach_task([this, func = std::move(func)] {
      const std::lock_guard<std::mutex> lock(mtx);

      tasks.push_back(std::move(func));
    });
  }

  fn wait() -> void
  {
    const std::lock_guard<std::mutex> lock(mtx);

    for (const auto& func : tasks) {
      func();
    }

    tasks.clear();
  }
};
}
