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
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

        //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtMemState memStart;
        _CrtMemCheckpoint(&memStart);
    #endif
#endif

        (void)getchar();

    Engine* engine = new Engine();

  if (engine->Initialize() != 0) {
    return -1;
  }

  auto res = engine->Run();
  
  
  delete engine;
#ifdef _WIN32
#ifdef DEBUG
  _CrtMemState memEnd, memDiff;
  _CrtMemCheckpoint(&memEnd);
  if (_CrtMemDifference(&memDiff, &memStart, &memEnd)) {
      _CrtMemDumpStatistics(&memDiff);
  }
#endif
#endif
  (void)getchar();
  return res;
}
