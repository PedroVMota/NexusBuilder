#include "Engine.h"
#include "Workspace.h"


#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main(int ac, char **av) {

  // Workspace works = Workspace(av[0]);
  //
  //
  //
  // works.loadWorkspace();
#ifdef _WIN32
#ifdef DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
    // Optional: break on specific allocation number
    // _CrtSetBreakAlloc(123);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
#endif
  Engine engine;

  if (engine.Initialize() != 0) {
    return -1;
  }

  return engine.Run();
}
