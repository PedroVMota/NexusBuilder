#pragma once


#include "Interfaces/GUIView.h"
#include "Workspace.h"


class GUIWorkspace : public GUIView{


private: 
	ImGuiIO  io;
	std::shared_ptr<Workspace> _spaceManager;


	void renderTemplateSection();
	void renderButtonSection();
	void renderWorkspaceSection(const std::vector<Project>&);
	void renderHeaderSection();
public:

	GUIWorkspace();
	GUIWorkspace(const GUIWorkspace &);
	GUIWorkspace &operator=(const GUIWorkspace &);
	virtual ~GUIWorkspace();

	void render();
	void destroy();
	void start();

	static GUIView *create();

};
