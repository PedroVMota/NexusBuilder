#ifndef GUICREATEPROJECT_H
#include "Interfaces/GUIView.h"
#include <Engine.h>
#include "GLFW/glfw3.h"

class GUICreateProject: public GUIView {
	private: 
	

	GLFWwindow *_selfWindow;



public:
	
	char * _workspaceFile = NULL;

	GUICreateProject();

	
	void render();
	void destroy();
	void start();

	virtual ~GUICreateProject();
	static GUIView *create(Engine* _ptr);
};



#endif
