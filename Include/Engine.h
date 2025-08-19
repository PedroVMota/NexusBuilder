#pragma once


#include "Editor.h"

class Engine{
public:
	Engine();
	~Engine();
	
	int Initialize();
	int Run();
	void Shutdown();

private:
	GLFWwindow* window;
	Editor* editor;

	
	bool InitializeGLFW();
	bool CreateWindow();
	bool InitializeOpenGL();
	void InitializeImGui();
	void LogGPUInfo();
	void SetupGLFWHints();
	void MainLoop();
};
