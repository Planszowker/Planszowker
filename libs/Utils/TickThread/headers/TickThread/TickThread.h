#pragma once

#include <ErrorHandler/ErrorLogger.h>

#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <atomic>

namespace pla::utils {

/**
 * @brief TickThread class. Used to unblock a thread when specified time has passed.
 * @details Tick event happens every %DURATION% time.
 *
 * @tparam TIMEBASE Type of std::chrono::duration to indicate time base.
 * @tparam DURATION Duration in which tick event occurs.
 *
 * @note This class throws an error when %DURATION% is 0.
 * @addtogroup non-copyable, non-movable
 */
template<typename TIMEBASE, size_t DURATION>
class [[maybe_unused]] TickThread {
public:
  TickThread()
    : m_duration(DURATION)
    , m_thread(std::jthread(&TickThread::tickFunction, this))
  {
    if (m_duration == 0) {
      err_handler::ErrorLogger::printError("[TickThread] Duration of TickThread cannot be 0!");
    }
  }

  ~TickThread()
  {
    m_runThread = false;

    // Wake up all threads waiting for tick event one last time.
    std::scoped_lock lock{m_mutex};
    m_cond.notify_all();
  }

  /**
   * @brief Block current thread and wait for tick event to occur.
   *
   */
  [[maybe_unused]] void waitForTick()
  {
    std::unique_lock<std::mutex> lock{m_mutex};

    // Wait for tick event to happen
    m_cond.wait(lock);
  }

  TickThread(const TickThread<TIMEBASE, DURATION>& other) noexcept = delete;
  TickThread(TickThread<TIMEBASE, DURATION>&& other) noexcept = delete;

  TickThread<TIMEBASE, DURATION>& operator=(const TickThread& other) noexcept = delete;
  TickThread<TIMEBASE, DURATION>& operator=(TickThread&& other) noexcept = delete;

private:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  void tickFunction()
  {
    while (m_runThread) {
      if (std::chrono::duration_cast<TIMEBASE>(std::chrono::high_resolution_clock::now() - m_timePassed).count() >= m_duration) {
        // We have passed set time
        std::scoped_lock lock{m_mutex};
        m_cond.notify_all();
      }

      std::this_thread::sleep_for(TIMEBASE(m_duration));
    }
  }

  Clock m_clock;
  TimePoint m_timePassed;

  size_t m_duration;

  std::mutex m_mutex;
  std::condition_variable m_cond;
  std::atomic<bool> m_runThread {true};

  std::jthread m_thread;
};

}