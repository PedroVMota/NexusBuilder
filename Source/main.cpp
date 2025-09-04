#include "Engine.h"
#include "Workspace.h"


#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main(int ac, char **av) {
#ifdef _WIN32
    #ifdef DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
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
