#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

int main(int argc, char* argv[]) {

  sol::state lua;
  lua.open_libraries(sol::lib::base,
                     sol::lib::package,
                     sol::lib::io,
                     sol::lib::table,
                     sol::lib::string);

  try {
    sol::protected_function_result result = lua.script_file("scripts/games/DiceRoller/DiceRoller-states.lua");
    sol::protected_function_result result2 = lua.script_file("scripts/games/DiceRoller/DiceRoller.lua");
  } catch(sol::error& e) {
    std::cerr << "Exception has been raised! " << e.what() << "\n";
  }

  std::cout << "Ending program...\n";
  return EXIT_SUCCESS;
}
