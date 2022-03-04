#include <string>
#include <functional>
#include <memory>

namespace pla::supervisor {

class Command
{
public:
  Command(std::string&& command, std::string&& description, std::function<void()> callback);

  inline std::string_view getCommand() { return m_command; }
  inline std::string_view getDescription() { return m_description; }

  inline void invoke() { m_callback(); }

private:
  std::function<void()> m_callback;
  std::string m_command;
  std::string m_description;
};

}