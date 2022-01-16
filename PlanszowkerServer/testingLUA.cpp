#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

int main(int argc, char* argv[]) {

  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);

  try {
    sol::protected_function_result result = lua.script_file("lua-scripts/games/DiceRoller.lua");
  } catch(sol::error& e) {
    std::cerr << "Exception has been raised! " << e.what() << "\n";
  }

  std::cout << "Ending program...\n";
  return EXIT_SUCCESS;
}
