#include "Command.h"

#include <utility>

namespace pla::server {

Command::Command(std::string&& command, std::string&& description, std::function<void()> callback)
  : m_command(std::move(command))
  , m_description(std::move(description))
  , m_callback(std::move(callback))
{
}

} // Namespace
