#include <TickThread/TickThread.h>

namespace pla::utils {

// Explicit TickThread instantiation
template<size_t DURATION> class [[maybe_unused]] TickThread<std::chrono::seconds, DURATION>;
template<size_t DURATION> class [[maybe_unused]] TickThread<std::chrono::milliseconds, DURATION>;

}