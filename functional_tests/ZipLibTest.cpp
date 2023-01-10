#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>

#include <ZipLib/ZipFile.h>

int main() {
  auto zipFile = ZipFile::Open("../../planszowker_server/scripts/games/DiceRoller.plagame");

  for (int idx = 0; idx < zipFile->GetEntriesCount(); ++idx) {
    std::string entryName = zipFile->GetEntry(idx)->GetFullName();
    std::cout << entryName << "\n";
    std::regex assetsRegex {"/Assets/"};
    if (std::regex_search(entryName, assetsRegex)) {
      std::cout << "Found asset " << entryName << "!\n";
    }
  }

  std::stringstream str{};
  zipFile->WriteToStream(str);

  return EXIT_SUCCESS;
}