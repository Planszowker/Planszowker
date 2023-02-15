#include <SharedObjects.h>

namespace pla::games_client::shared {

games::GamesMetaInfo& getGamesMetaInfo() {
  static games::GamesMetaInfo gamesMetaInfo;
  return gamesMetaInfo;
}

} // namespace