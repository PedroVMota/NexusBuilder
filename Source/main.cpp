#include "Engine.h"
#include "Workspace.h"

int main(int ac, char **av) {

  // Workspace works = Workspace(av[0]);
  //
  //
  //
  // works.loadWorkspace();
  Engine engine;

  if (engine.Initialize() != 0) {
    return -1;
  }

  return engine.Run();
}
