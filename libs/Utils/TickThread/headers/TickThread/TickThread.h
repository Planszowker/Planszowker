#pragma once

#include <ErrorHandler/ErrorLogger.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace pla::utils {

/**
 * @brief TickThread class. Used to unblock a thread when specified time has passed.
 * @details Tick event happens every %DURATION% time. Minimal quantization is 10ms.
 *
 * @tparam TIMEBASE Type of std::chrono::duration to indicate time base. Currently only timebase bigger than ms are supported.
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
    , m_timePassed(Clock::now())
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
   */
  [[maybe_unused]] void waitForTick()
  {
    std::unique_lock<std::mutex> lock{m_mutex};

    // Wait for tick event to happen
    m_cond.wait(lock);
  }

  /**
   * @brief Check if tick even has occurred.
   *
   * @return True if tick even occurred, false otherwise.
   */
  [[maybe_unused]] bool checkIfTick()
  {
    std::unique_lock<std::mutex> lock{m_mutex};

    // Check if tick even happened
    auto timeout = m_cond.wait_for(lock, MinimalQuantizationInMs);

    return timeout == std::cv_status::no_timeout;
  }

  TickThread(const TickThread<TIMEBASE, DURATION>& other) noexcept = delete;
  TickThread(TickThread<TIMEBASE, DURATION>&& other) noexcept = delete;

  TickThread<TIMEBASE, DURATION>& operator=(const TickThread& other) noexcept = delete;
  TickThread<TIMEBASE, DURATION>& operator=(TickThread&& other) noexcept = delete;

private:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  static constexpr auto MinimalQuantizationInMs = std::chrono::milliseconds(10);

  void tickFunction()
  {
    while (m_runThread) {
      if (std::chrono::duration_cast<TIMEBASE>(std::chrono::high_resolution_clock::now() - m_timePassed).count() >= m_duration) {
        // We have passed set time
        std::scoped_lock lock{m_mutex};
        m_cond.notify_all();
        m_timePassed = Clock::now();
      }

      std::this_thread::sleep_for(MinimalQuantizationInMs);
    }
  }

  TimePoint m_timePassed;

  size_t m_duration;

  std::mutex m_mutex;
  std::condition_variable m_cond;
  std::atomic<bool> m_runThread {true};

  std::jthread m_thread;
};

}