#include "Engine.h"

int main()
{

	Engine engine;
	
	if (engine.Initialize() != 0) {
		return -1;
	}
	
	return engine.Run();
}
