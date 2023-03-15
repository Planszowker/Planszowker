#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

namespace pla::utils {

template<typename T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue() = default;
  ~ThreadSafeQueue() = default;
//  ThreadSafeQueue(const ThreadSafeQueue& other) = delete; // Copy constructor
//  ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete; // Copy assignment
//
//  ThreadSafeQueue(ThreadSafeQueue&& other) noexcept; // Move constructor
//  ThreadSafeQueue& operator=(ThreadSafeQueue&& other) noexcept; // Move assignment

  ThreadSafeQueue(ThreadSafeQueue<T>&& other) noexcept
  {
    std::unique_lock<std::mutex> lockThis(this->m_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lockOther(other.m_mutex, std::defer_lock);
    std::lock(lockThis, lockOther);
    this->m_queue = std::move(other.m_queue);
  }

  ThreadSafeQueue<T>& operator=(ThreadSafeQueue&& other) noexcept
  {
    std::unique_lock<std::mutex> lockThis(this->m_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lockOther(other.m_mutex, std::defer_lock);
    std::lock(lockThis, lockOther);
    this->m_queue = std::move(other.m_queue);

    return *this;
  }

  void push(const T& var)
  {
    std::lock_guard<std::mutex> lock{m_mutex};
    m_queue.push(var);
    m_cond.notify_one();
  }

  T pop()
  {
    std::unique_lock<std::mutex> lock{m_mutex};

    // Wait until queue is not empty
    m_cond.wait(lock, [this]() { return not m_queue.empty(); });

    T item = m_queue.front();
    m_queue.pop();

    return item;
  }

//  void push(const T& var);
//  T pop();
private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

}