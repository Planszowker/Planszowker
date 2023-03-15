#include <ThreadSafeQueue.h>

#include <Games/Objects.h>

namespace pla::utils {

//template <typename T>
//ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue<T>&& other) noexcept
//{
//  std::unique_lock<std::mutex> lockThis(this->m_mutex, std::defer_lock);
//  std::unique_lock<std::mutex> lockOther(other.m_mutex, std::defer_lock);
//  std::lock(lockThis, lockOther);
//  this->m_queue = std::move(other.m_queue);
//}
//
//
//template<typename T>
//ThreadSafeQueue<T>& ThreadSafeQueue<T>::operator=(ThreadSafeQueue&& other) noexcept
//{
//  std::unique_lock<std::mutex> lockThis(this->m_mutex, std::defer_lock);
//  std::unique_lock<std::mutex> lockOther(other.m_mutex, std::defer_lock);
//  std::lock(lockThis, lockOther);
//  this->m_queue = std::move(other.m_queue);
//
//  return *this;
//}
//
//
//template <typename T>
//void ThreadSafeQueue<T>::push(const T& var)
//{
//  std::lock_guard<std::mutex> lock{m_mutex};
//  m_queue.push(var);
//  m_cond.notify_one();
//}
//
//
//template <typename T>
//T ThreadSafeQueue<T>::pop()
//{
//  std::unique_lock<std::mutex> lock{m_mutex};
//
//  // Wait until queue is not empty
//  m_cond.wait(lock, [this]() { return not m_queue.empty(); });
//
//  T item = m_queue.front();
//  m_queue.pop();
//
//  return item;
//}
//
//// Explicit template instantiation
//template class ThreadSafeQueue<pla::games::Reply>;

}