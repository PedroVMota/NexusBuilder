#pragma once


#include "Interfaces/GUIView.h"
#include "Workspace.h"
#include "Engine.h"
#include <GLFW/glfw3.h>

class GUIWorkspace : public GUIView{


private: 
	ImGuiIO  io;
	std::shared_ptr<Workspace> _spaceManager;
	Engine *engine;


	void renderTemplateSection();
	void renderButtonSection();
	void renderWorkspaceSection(const std::vector<Project>&);
	void renderHeaderSection();



	GUIView *view = NULL;
	unsigned int _subWindowsLength = NULL;

	



public:

	GUIWorkspace(Engine *_engine_ptr);
	GUIWorkspace(const GUIWorkspace &);
	GUIWorkspace &operator=(const GUIWorkspace &);
	virtual ~GUIWorkspace();

	void render();
	void destroy();
	void start();

	static GUIView *create(Engine* _ptr);

};
