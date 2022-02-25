#include "ServerSupervisor.h"

#include <PlametaParser/Entry.h>

#include <string>
#include <iostream>
#include <thread>

namespace pla::server {

ServerSupervisor::ServerSupervisor(std::ifstream& configStream)
  : m_configStream(configStream)
  , m_configParser(m_configStream)
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


void ServerSupervisor::run()
{
  // Do something...

  std::shared_ptr<utils::plameta::Entry> entryPtr = m_configParser["config:port"];

  if (entryPtr) {
    std::cout << "[" << entryPtr->getKey() << "]: ";
    switch (entryPtr->getType()) {
      case utils::plameta::EntryType::Int:
        std::cout << entryPtr->getValue<int>();
        break;
      case utils::plameta::EntryType::String:
        std::cout << entryPtr->getValue<std::string>();
        break;
      case utils::plameta::EntryType::Float:
        std::cout << entryPtr->getValue<float>();
        break;
      default:
        LOG(ERROR) << "Wrong entry type!";
        std::terminate();
        break;
    }
    std::cout << "\n";
  }

  std::thread inputThread {&ServerSupervisor::_getUserInput, this};

  inputThread.join();
}


void ServerSupervisor::_registerCommand(std::shared_ptr<Command>&& command)
{
  m_commands.push_back(std::move(command));
}


void ServerSupervisor::_getUserInput()
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
