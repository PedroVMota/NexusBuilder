#pragma once


#include "Interfaces/GUIView.h"



class GUIWorkspace : public GUIView{


private: 
	ImGuiIO  io;

public: 

	GUIWorkspace();
	GUIWorkspace(const GUIWorkspace &);
	GUIWorkspace &operator=(const GUIWorkspace &);
	virtual ~GUIWorkspace();

	void render() const;
	void destroy();
	void start();

	static GUIView *create();

};
