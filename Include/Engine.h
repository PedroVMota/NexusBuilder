#pragma once


// #include "Editor.h"
#include <memory>
#include "Workspace.h"
#include "Interfaces/GUIView.h"

enum EngineState {
	CHOOSING_WORKSPACE = -1,
	RUNNING,
	CLOSE
};

class Engine{
public:
	Engine();
	Engine(const Engine&);
	~Engine();
	
	int Initialize();
	int Run();
	void Shutdown();


	void setRuning(EngineState);
private:
	GLFWwindow* window;
	GUIView* editor;
	EngineState state;
	std::vector<std::string> _createdWorksSpaces;



	bool renderWorkspaceManager();
	bool InitializeGLFW();
	bool CreateWindow();
	bool InitializeOpenGL();
	void InitializeImGui();
	void LogGPUInfo();
	void SetupGLFWHints();
	void MainLoop();
};
