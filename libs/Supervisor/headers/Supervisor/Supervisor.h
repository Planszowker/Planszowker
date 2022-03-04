#include "Command.h"
#include "GamesInfoExtractor.h"

#include <PlametaParser/Parser.h>
#include <easylogging++.h>

#include <atomic>
#include <memory>
#include <sstream>

namespace pla::supervisor {

class Supervisor
{
public:
  Supervisor() = delete;
  explicit Supervisor(std::stringstream configStream);

  void run();

private:
  void _getUserInput();
  void _registerCommand(std::shared_ptr<Command>&& command);

  utils::plameta::Parser m_configParser;

  std::atomic_bool m_run {true};

  std::vector<std::shared_ptr<Command>> m_commands;

  GamesInfoExtractor ziuziu;
};

}