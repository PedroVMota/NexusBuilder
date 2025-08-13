#pragma once

#include "main.h"
#include "Camera.h"
#include "Light.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "ObjectManager.h"
#include "Primitives.h"
#include <memory>
#include <vector>
#include <unordered_map>

// Forward declarations
class Camera;
class Light;
class GameObject;

// Renderable component that can be attached to GameObjects
class MeshRenderer {
private:
	Mesh mesh;
	bool enabled;
	
public:
	MeshRenderer() : enabled(true) {}
	MeshRenderer(const Mesh& mesh) : mesh(mesh), enabled(true) {}
	
	void SetMesh(const Mesh& newMesh) { mesh = newMesh; }
	const Mesh& GetMesh() const { return mesh; }
	
	void SetEnabled(bool enable) { enabled = enable; }
	bool IsEnabled() const { return enabled; }
	
	void Render(const Mat4& modelMatrix, const Mat4& viewMatrix, const Mat4& projectionMatrix, 
				const std::vector<Light*>& lights);
};

class Scene {
private:
	// Core rendering components
	std::unique_ptr<Camera> m_camera;
	std::vector<Light*> m_lights;
	std::unordered_map<GameObject*, std::unique_ptr<MeshRenderer>> m_meshRenderers;
	
	// Rendering state
	Vec3 m_backgroundColor;
	bool m_depthTestEnabled;
	bool m_backfaceCullingEnabled;
	bool m_wireframeMode;
	
	// Shader management
	std::unique_ptr<Shader> m_defaultShader;
	GLuint m_shaderProgram;
	
	// Scene management
	bool m_needsUpdate;
	float m_deltaTime;
	
public:
	Scene();
	~Scene();
	
	// Core rendering
	void Draw();
	void Update(float deltaTime);
	void Clear();
	
	// Camera management
	void SetCamera(std::unique_ptr<Camera> camera);
	Camera* GetCamera() const { return m_camera.get(); }
	Camera* CreateDefaultCamera();
	
	// Light management
	void AddLight(Light* light);
	void RemoveLight(Light* light);
	Light* CreateDirectionalLight(const String& name = "DirectionalLight", 
								  const Vec3& direction = Vec3(0.0f, -1.0f, 0.0f));
	Light* CreatePointLight(const String& name = "PointLight", 
						   const Vec3& position = Vec3(0.0f, 2.0f, 0.0f));
	Light* CreateSpotLight(const String& name = "SpotLight", 
						  const Vec3& position = Vec3(0.0f, 2.0f, 0.0f),
						  const Vec3& direction = Vec3(0.0f, -1.0f, 0.0f));
	const std::vector<Light*>& GetLights() const { return m_lights; }
	
	// GameObject and mesh management
	GameObject* CreateGameObject(const String& name = "GameObject");
	GameObject* CreateGameObject(const String& name, const Vec3& position);
	void AddMeshToGameObject(GameObject* gameObject, const Mesh& mesh);
	void AddMeshToGameObject(GameObject* gameObject, const Mesh& mesh, const Material& material);
	void RemoveMeshFromGameObject(GameObject* gameObject);
	MeshRenderer* GetMeshRenderer(GameObject* gameObject);
	
	// Primitive creation helpers
	GameObject* CreateCube(const String& name = "Cube", const Vec3& position = Vec3(0.0f), 
						  float size = 1.0f, const Material& material = Material::CreateDefaultMaterial());
	GameObject* CreateSphere(const String& name = "Sphere", const Vec3& position = Vec3(0.0f), 
						   float radius = 0.5f, const Material& material = Material::CreateDefaultMaterial());
	GameObject* CreatePlane(const String& name = "Plane", const Vec3& position = Vec3(0.0f), 
						  float width = 1.0f, float height = 1.0f, 
						  const Material& material = Material::CreateDefaultMaterial());
	GameObject* CreateCylinder(const String& name = "Cylinder", const Vec3& position = Vec3(0.0f), 
							 float radius = 0.5f, float height = 1.0f, 
							 const Material& material = Material::CreateDefaultMaterial());
	
	// Simplified primitive creation methods (always use default material)
	GameObject* CreateCubeSimple(const String& name, const Vec3& position, float size = 1.0f);
	GameObject* CreateSphereSimple(const String& name, const Vec3& position, float radius = 0.5f);
	GameObject* CreatePlaneSimple(const String& name, const Vec3& position, float width = 1.0f, float height = 1.0f);
	GameObject* CreateCylinderSimple(const String& name, const Vec3& position, float radius = 0.5f, float height = 1.0f);
	
	// Material management for existing objects
	void SetObjectMaterial(GameObject* gameObject, const Material& material);
	Material GetObjectMaterial(GameObject* gameObject) const;
	void SetObjectMaterialToDefault(GameObject* gameObject);
	
	// Material preset helpers
	void SetObjectMaterialToMetal(GameObject* gameObject);
	void SetObjectMaterialToPlastic(GameObject* gameObject);
	void SetObjectMaterialToGlass(GameObject* gameObject);
	void SetObjectMaterialColor(GameObject* gameObject, const Vec3& color);
	
	// Scene settings
	void SetBackgroundColor(const Vec3& color) { m_backgroundColor = color; }
	const Vec3& GetBackgroundColor() const { return m_backgroundColor; }
	
	void SetDepthTestEnabled(bool enabled) { m_depthTestEnabled = enabled; }
	bool IsDepthTestEnabled() const { return m_depthTestEnabled; }
	
	void SetBackfaceCullingEnabled(bool enabled) { m_backfaceCullingEnabled = enabled; }
	bool IsBackfaceCullingEnabled() const { return m_backfaceCullingEnabled; }
	
	void SetWireframeMode(bool enabled) { m_wireframeMode = enabled; }
	bool IsWireframeMode() const { return m_wireframeMode; }
	
	// Utility
	void ResetScene();
	void FrameAll(); // Frames all objects in the scene
	size_t GetRenderableCount() const { return m_meshRenderers.size(); }
	size_t GetLightCount() const { return m_lights.size(); }
	
private:
	// Internal rendering helpers
	void SetupDefaultShader();
	void ApplyRenderingSettings();
	void RenderMeshes();
	void SetupLighting();
	void CleanupDestroyedObjects();
	
	// Shader uniform helpers
	void SetShaderUniforms(const Mat4& modelMatrix, const Mat4& viewMatrix, const Mat4& projectionMatrix);
	void SetLightingUniforms();
	void SetMaterialUniforms(const Material& material);
};
