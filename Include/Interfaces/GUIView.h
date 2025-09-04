#pragma once 



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


class GUIView {
public:
	virtual ~GUIView() {}
	virtual void start()  = 0;
	virtual void render()  = 0;
	virtual void destroy() = 0;


};
