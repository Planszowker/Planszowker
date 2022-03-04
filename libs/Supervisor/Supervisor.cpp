#include <Supervisor/Supervisor.h>
#include <PlametaParser/Entry.h>
#include <NetworkHandler/ServerPacketHandler.h>

#include <string>
#include <iostream>
#include <thread>

namespace pla::supervisor {

Supervisor::Supervisor(std::stringstream configStream)
  : m_configParser(std::move(configStream))
{
  auto helpCmd = std::make_shared<Command>(
          "help",
          "List all available commands",
          [this]()
          {
            std::cout << "Available commands:\n";
            for (const auto& command : this->m_commands) {
              std::cout << "\t" << command->getCommand() << " - " << command->getDescription() << "\n";
            }
          }
  );

  auto quitCmd = std::make_shared<Command>(
          "quit",
          "Stops server and all game instances",
          [this]()
          {
            this->m_run = false;
          }
  );

  _registerCommand(std::move(helpCmd));
  _registerCommand(std::move(quitCmd));
}


void Supervisor::run()
{
  // Do something...

  auto entryPtr = m_configParser["config:port"];
  std::cout << "[Config]:port = " << std::get<int>(entryPtr->getVariant()) << "\n";

  std::size_t port = static_cast<size_t>(std::get<int>(entryPtr->getVariant()));
  network::SupervisorPacketHandler supervisorPacketHandler {m_run, port};

  supervisorPacketHandler.runInBackground();

  std::thread inputThread {&Supervisor::_getUserInput, this};

  inputThread.join();
}


void Supervisor::_registerCommand(std::shared_ptr<Command>&& command)
{
  m_commands.push_back(std::move(command));
}


void Supervisor::_getUserInput()
{
  std::string inputCommand;

  std::cout << "Planszówker Server v" << PROJECT_VER << "\n"
                                                        "(Type \'help\' for list of all available commands)\n"
                                                        "================================================\n";

  while (m_run) {
    std::cout << "Command: ";

    std::cin >> inputCommand;

    bool cmdFound {false};
    for (const auto& command : m_commands) {
      if (command->getCommand().compare(inputCommand) == 0) {
        command->invoke();
        cmdFound = true;
        break;
      }
    }

    if (!cmdFound) {
      std::cout << "Command not found!\n";
    }
  }
}

} // namespace
