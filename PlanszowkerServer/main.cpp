#include <string>

#include <ErrorHandler/ExceptionThrower.h>
#include <Supervisor/Supervisor.h>

#include <easylogging++.h>

#define ELPP_THREAD_SAFE // Set multithreading for EasyLogging

using namespace pla;
using namespace pla::supervisor;
using namespace pla::err_handler;

INITIALIZE_EASYLOGGINGPP

static constexpr auto CONFIG_FILENAME = "config.plameta";

////////////
// SERVER //
////////////
int main() {

  std::ifstream configStream {CONFIG_FILENAME};

  el::Configurations customConf;
  customConf.setToDefault();
  customConf.set(el::Level::Debug, el::ConfigurationType::Format, "[%level]: %msg");
  el::Loggers::reconfigureLogger("default", customConf);

  if (!configStream.is_open()) {
    LOG(ERROR) << "Cannot open " << CONFIG_FILENAME << " file!";
    return EXIT_FAILURE;
  }

  try {
    Supervisor serverSupervisor {configStream};
    serverSupervisor.run();
  } catch (ExceptionThrower& e) {
    LOG(INFO) << "Terminating Planszówker Server due to errors...";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
