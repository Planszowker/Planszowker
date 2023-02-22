#include <SharedObjects.h>

namespace pla::games_client::shared {

games::GamesMetaInfo& getGamesMetaInfo() {
  static games::GamesMetaInfo gamesMetaInfo;
  return gamesMetaInfo;
}

client_info::ClientInfo& getClientInfo() {
  static client_info::ClientInfo clientInfo{"127.0.0.1", 0, 0};
  return clientInfo;
}

} // namespace