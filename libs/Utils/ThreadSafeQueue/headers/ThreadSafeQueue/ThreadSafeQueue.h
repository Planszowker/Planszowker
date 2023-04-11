#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <optional>

namespace pla::utils {

template<typename T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue() = default;
  ~ThreadSafeQueue() = default;

  ThreadSafeQueue(const ThreadSafeQueue<T>& other) noexcept = delete;
  ThreadSafeQueue(ThreadSafeQueue<T>&& other) noexcept = delete;

  ThreadSafeQueue<T>& operator=(const ThreadSafeQueue& other) noexcept = delete;
  ThreadSafeQueue<T>& operator=(ThreadSafeQueue&& other) noexcept = delete;

  void push(const T& var)
  {
    std::scoped_lock lock{m_mutex};
    m_queue.push(var);
    m_cond.notify_one();
  }

  std::optional<T> pop()
  {
    std::unique_lock<std::mutex> lock{m_mutex};

    // Wait until queue is not empty
    if (m_cond.wait_for(lock, std::chrono::milliseconds(10), [this]() { return not m_queue.empty(); })) {
      T item = m_queue.front();
      m_queue.pop();

      return item;
    } else {
      // Timeout
      return std::nullopt;
    }
  }

private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

}