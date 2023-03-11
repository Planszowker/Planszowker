#include <ThreadSafeQueue.h>

namespace pla::utils {

template <typename T>
void ThreadSafeQueue<T>::push(const T& var)
{
  std::lock_guard<std::mutex> lock{m_mutex};
  m_queue.push(var);
  m_cond.notify_one();
}


template <typename T>
T ThreadSafeQueue<T>::pop()
{
  std::lock_guard<std::mutex> lock{m_mutex};

  // Wait until queue is not empty
  m_cond.wait(lock, [this]() { return not m_queue.empty(); });

  T item = m_queue.front();
  m_queue.pop();

  return item;
}

}