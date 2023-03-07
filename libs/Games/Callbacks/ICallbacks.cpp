#include <Games/Callbacks/ICallbacks.h>

#include <Games/Objects.h>
#include <ErrorHandler/ErrorLogger.h>
#include <GamesClient/SharedObjects.h>
#include <nlohmann/json.hpp>
#include <easylogging++.h>

namespace pla::games {

using namespace games_client;
using namespace json_entries;

void ICallbacks::IDCallback(std::any &arg)
{
  try {
    auto callbackReplyJson = nlohmann::json::parse(std::any_cast<std::string>(arg));
    LOG(DEBUG) << "[ICallbacks]::IDCallback: Setting new Client ID " << callbackReplyJson[CLIENT_ID];
    shared::getClientInfo().setId(callbackReplyJson[CLIENT_ID]);
  } catch (std::bad_any_cast& e) {
    err_handler::ErrorLogger::printError("[ICallbacks] Bad any cast!");
  }
}

}
