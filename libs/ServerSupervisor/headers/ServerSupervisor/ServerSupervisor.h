#include "Command.h"

#include <PlametaParser/Parser.h>
#include <easylogging++.h>

#include <atomic>
#include <memory>
#include <fstream>

namespace pla::server {

class ServerSupervisor
{
public:
  ServerSupervisor() = delete;
  explicit ServerSupervisor(std::ifstream& configStream);

  void run();

private:
  void _getUserInput();
  void _registerCommand(std::shared_ptr<Command>&& command);

  std::ifstream& m_configStream;
  utils::plameta::Parser m_configParser;

  std::atomic_bool m_run {true};

  std::vector<std::shared_ptr<Command>> m_commands;
};

}