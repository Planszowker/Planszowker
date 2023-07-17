#include <Callbacks/GameCallbacks.h>

#include <Games/CommObjects.h>
#include <Games/BoardParser.h>
#include <AssetsManager/AssetsReceiver.h>
#include <ErrorHandler/ErrorLogger.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

namespace pla::games {

using namespace json_entries;

void GameCallbacks::downloadAssetsCallback(const std::any& arg)
{
  try {
    LOG(DEBUG) << "[GameCallbacks] downloadAssetsCallback";
    LOG(DEBUG) << "[GameCallbacks] parsing BoardDescription";
    m_state.m_boardParser = std::move(std::make_shared<BoardParser>(assets::AssetsReceiver::getBoardDescription()));
  } catch (std::exception& e) {
    err_handler::ErrorLogger::printError("[GameCallbacks] Bad any cast!");
  }
}


void GameCallbacks::gameSpecificDataCallback(const std::any& arg)
{
  try {
    if (m_state.m_boardParser) {
      auto replyJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
      if (replyJson.value(VALID, false)) {
        m_state.m_boardParser->updateObjects(replyJson);
      }
    }
  } catch (std::exception& e) {
    LOG(ERROR) << e.what();
    err_handler::ErrorLogger::printError("[GameCallbacks] Bad any cast!");
  }
}

}