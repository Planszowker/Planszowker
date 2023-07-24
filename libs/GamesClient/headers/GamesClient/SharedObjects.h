#pragma once

#include <Games/GamesMetaInfo.h>
#include <ClientInfo/ClientInfo.h>

namespace pla::games_client::shared {

games::GamesMetaInfo& getGamesMetaInfo();
client_info::ClientInfo& getClientInfo();

} //namespace