#pragma once


#include "Interfaces/GUIView.h"
#include "Workspace.h"
#include "Engine.h"


class GUIWorkspace : public GUIView{


private: 
	ImGuiIO  io;
	std::shared_ptr<Workspace> _spaceManager;
	Engine *engine;


	void renderTemplateSection();
	void renderButtonSection();
	void renderWorkspaceSection(const std::vector<Project>&);
	void renderHeaderSection();



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
