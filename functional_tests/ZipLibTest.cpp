#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iomanip>

#include <ZipLib/ZipFile.h>

#include <base64.hpp>

int main() {
  auto zipFile = ZipFile::Open("../planszowker_server/scripts/games/DiceRoller.plagame");

  for (int idx = 0; idx < zipFile->GetEntriesCount(); ++idx) {
    auto entry = zipFile->GetEntry(idx);
    std::string entryName = entry->GetFullName();
    std::cout << entryName << "\n";
    std::regex assetsRegex {"/Assets/.+"};
    if (std::regex_search(entryName, assetsRegex)) {
      std::cout << "Found asset " << entryName << "!\n";
      auto stream = entry->GetDecompressionStream();
      stream->unsetf(std::ios_base::skipws);
      std::string s{std::istream_iterator<char>{*stream}, {}};
      std::cout << "Base64: " << base64::encode(s) << "\n";

      std::shared_ptr<std::string> stringPtr = std::make_shared<std::string>(base64::decode(base64::encode(s)));

      std::ofstream ofs;
      ofs.open(entry->GetName(), std::ios::binary | std::ios::out);
      if (!ofs) {
        continue;
      }
      ofs << s;
      ofs.close();

      sf::RenderWindow window(sf::VideoMode(400, 400), entryName);
      std::shared_ptr<sf::Texture> txt = std::make_shared<sf::Texture>();
      txt->loadFromMemory(stringPtr->c_str(), stringPtr->size());
      while(window.isOpen()){
        sf::Event event{};
        while(window.pollEvent(event)){
          if (event.type == sf::Event::Closed) {
            window.close();
          }

          if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
            std::cout << "StringPtr was poiting to " << stringPtr << "\n";
            stringPtr.reset();
            std::cout << "StringPtr is poiting to " << stringPtr << "\n";
          }
        }

        sf::Sprite spr;
        spr.setTexture(*txt);
        window.clear();
        window.draw(spr);
        window.display();
      }
    }
  }

  return EXIT_SUCCESS;
}