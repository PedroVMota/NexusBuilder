#pragma once

#include "main.h"
#include <memory>

// Forward declarations
class Scene;

class Viewport {
public:
	Viewport();
	~Viewport();
	void Render();
	void Resize(int width, int height);
	unsigned int GetFramebufferTexture() const { return m_colorTexture; }
	
	// Scene access
	Scene* GetScene() const { return m_scene.get(); }
	
private:
	void CreateFramebuffer();
	void DeleteFramebuffer();
	void InitializeScene();
	
	unsigned int m_framebuffer = 0;
	unsigned int m_colorTexture = 0;
	unsigned int m_depthRenderbuffer = 0;
	int m_width = 800;
	int m_height = 600;
	bool m_framebufferValid = false;
	
	// Scene for 3D rendering
	std::unique_ptr<Scene> m_scene;
};