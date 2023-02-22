#include <Games/Callbacks/ICallbacks.h>

#include <ErrorHandler/ErrorLogger.h>
#include <GamesClient/SharedObjects.h>
#include <nlohmann/json.hpp>
#include <easylogging++.h>

namespace pla::games {

using namespace games_client;

void ICallbacks::IDCallback(std::any &arg)
{
  try {
    auto callbackReplyJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
    LOG(DEBUG) << "[ICallbacks]::IDCallback: Setting new Client ID " << callbackReplyJson["ClientID"];
    shared::getClientInfo().setId(callbackReplyJson["ClientID"]);
  } catch (std::bad_any_cast& e) {
    err_handler::ErrorLogger::printError("[ICallbacks] Bad any cast!");
  }
}

}
