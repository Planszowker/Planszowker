#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <regex>

#include <zipios/zipfile.hpp>

int main() {
  zipios::ZipFile zipFile("../PlanszowkerServer/scripts/games/DiceRoller.plagame");

  for (auto entry : zipFile.entries()) {
    //std::cout << entry->getName() << "\n";
    std::regex assetsRegex {"/Assets/"};
    if (std::regex_search(entry->getName(), assetsRegex)) {
      std::cout << "Found asset " << entry->getFileName() << "!\n";
    }
  }

  //zipios::ZipFile::stream_pointer_t is(zipFile.getInputStream(entry->getName()));

  //std::cout << is->rdbuf() << "\n";

  return EXIT_SUCCESS;
}