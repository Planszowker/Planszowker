#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

namespace pla::utils {

template<typename T>
class ThreadSafeQueue {
public:
  void push(const T& var);
  T pop();
private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

}