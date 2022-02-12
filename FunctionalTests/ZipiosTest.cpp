#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include <zipios/zipfile.hpp>

int main() {
  //std::fstream file{"scripts/games/DiceRoller.zip"};

  zipios::ZipFile zipFile("./scripts/games/DiceRoller.plagame");

  zipios::FileEntry::pointer_t entry = zipFile.getEntry(("DiceRoller/DiceRoller.lua"));

  zipios::ZipFile::stream_pointer_t is(zipFile.getInputStream(entry->getName()));

  std::cout << is->rdbuf() << "\n";

  return EXIT_SUCCESS;
}