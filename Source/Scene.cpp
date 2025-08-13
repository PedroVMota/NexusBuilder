#include "Scene.h"
#include "Material.h"

// MeshRenderer Implementation
void MeshRenderer::Render(const Mat4& modelMatrix, const Mat4& viewMatrix, const Mat4& projectionMatrix, 
						 const std::vector<Light*>& lights) {
	if (!enabled || !mesh.isValid()) {
		return;
	}
	
	// The mesh will handle its own rendering with the current shader
	// Matrix uniforms should be set by the Scene before calling this
	mesh.draw();
}

// Scene Implementation
Scene::Scene() 
	: m_backgroundColor(0.4f, 0.6f, 0.8f)  // Light blue background
	, m_depthTestEnabled(true)
	, m_backfaceCullingEnabled(true)
	, m_wireframeMode(false)
	, m_needsUpdate(false)
	, m_deltaTime(0.0f)
	, m_shaderProgram(0) {
	
	// Create default camera
	CreateDefaultCamera();
	
	// Setup default shader
	SetupDefaultShader();
	
	BOTAPICA_LOG_INFO("Scene initialized");
}

Scene::~Scene() {
	// Cleanup shader
	if (m_shaderProgram != 0) {
		glDeleteProgram(m_shaderProgram);
	}
	
	// Clear all mesh renderers
	m_meshRenderers.clear();
	
	BOTAPICA_LOG_INFO("Scene destroyed");
}

void Scene::Draw() {
	// Apply rendering settings
	ApplyRenderingSettings();
	
	// Clear the screen
	glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Check if we have a camera
	if (!m_camera) {
		BOTAPICA_LOG_WARN("No camera set for scene rendering");
		return;
	}
	
	// Get view and projection matrices
	Mat4 viewMatrix = m_camera->GetViewMatrix();
	Mat4 projectionMatrix = m_camera->GetProjectionMatrix();
	
	// Use our shader program
	if (m_shaderProgram != 0) {
		glUseProgram(m_shaderProgram);
		
		// Set up lighting uniforms
		SetupLighting();
		
		// Render all mesh renderers
		if (m_meshRenderers.empty()) {
			static bool warned = false;
			if (!warned) {
				BOTAPICA_LOG_WARN("No mesh renderers to render in scene");
				warned = true;
			}
		} else {
			static bool logged = false;
			if (!logged) {
				BOTAPICA_LOG_INFO("Rendering " + std::to_string(m_meshRenderers.size()) + " mesh renderers");
				logged = true;
			}
		}
		RenderMeshes();
		
		glUseProgram(0);
	} else {
		static bool warned = false;
		if (!warned) {
			BOTAPICA_LOG_ERROR("No shader program available for rendering");
			warned = true;
		}
	}
	
	// Clean up any destroyed objects
	CleanupDestroyedObjects();
}

void Scene::Update(float deltaTime) {
	m_deltaTime = deltaTime;
	
	// Update camera if it exists
	if (m_camera) {
		// Camera updates are typically handled by input systems
	}
	
	// Update all lights
	for (Light* light : m_lights) {
		if (light && light->IsActive()) {
			light->Update(deltaTime);
		}
	}
	
	// Update all game objects through the ObjectManager
	const std::vector<Object*>& allObjects = ObjectManager::GetAllObjects();
	for (Object* obj : allObjects) {
		GameObject* gameObj = dynamic_cast<GameObject*>(obj);
		if (gameObj && gameObj->IsActive()) {
			gameObj->Update(deltaTime);
		}
	}
	
	m_needsUpdate = true;
}

void Scene::Clear() {
	glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Camera Management
void Scene::SetCamera(std::unique_ptr<Camera> camera) {
	m_camera = std::move(camera);
	BOTAPICA_LOG_INFO("Camera set for scene");
}

Camera* Scene::CreateDefaultCamera() {
	m_camera = std::make_unique<Camera>(Vec3(0.0f, 0.0f, 3.0f));
	m_camera->SetAspectRatio(static_cast<float>(WIDHT) / static_cast<float>(HEIGHT));
	BOTAPICA_LOG_INFO("Default camera created for scene");
	return m_camera.get();
}

// Light Management
void Scene::AddLight(Light* light) {
	if (light) {
		m_lights.push_back(light);
		BOTAPICA_LOG_INFO("Light added to scene: " + light->GetName());
	}
}

void Scene::RemoveLight(Light* light) {
	auto it = std::find(m_lights.begin(), m_lights.end(), light);
	if (it != m_lights.end()) {
		m_lights.erase(it);
		BOTAPICA_LOG_INFO("Light removed from scene");
	}
}

Light* Scene::CreateDirectionalLight(const String& name, const Vec3& direction) {
	Light* light = Light::CreateDirectionalLight(name);
	light->SetDirection(direction);
	light->SetColor(Vec3(1.0f, 1.0f, 1.0f));
	light->SetIntensity(1.0f);
	AddLight(light);
	return light;
}

Light* Scene::CreatePointLight(const String& name, const Vec3& position) {
	Light* light = Light::CreatePointLight(name, position);
	light->SetColor(Vec3(1.0f, 1.0f, 1.0f));
	light->SetIntensity(1.0f);
	light->SetAttenuation(1.0f, 0.09f, 0.032f);
	AddLight(light);
	return light;
}

Light* Scene::CreateSpotLight(const String& name, const Vec3& position, const Vec3& direction) {
	Light* light = Light::CreateSpotLight(name, position, direction);
	light->SetColor(Vec3(1.0f, 1.0f, 1.0f));
	light->SetIntensity(1.0f);
	light->SetAttenuation(1.0f, 0.09f, 0.032f);
	light->SetSpotCone(12.5f, 15.0f);
	AddLight(light);
	return light;
}

// GameObject and Mesh Management
GameObject* Scene::CreateGameObject(const String& name) {
	return GameObject::CreateGameObject(name);
}

GameObject* Scene::CreateGameObject(const String& name, const Vec3& position) {
	return GameObject::CreateGameObject(name, position);
}

void Scene::AddMeshToGameObject(GameObject* gameObject, const Mesh& mesh) {
	if (gameObject) {
		m_meshRenderers[gameObject] = std::make_unique<MeshRenderer>(mesh);
		BOTAPICA_LOG_INFO("Mesh added to GameObject: " + gameObject->GetName());
	}
}

void Scene::AddMeshToGameObject(GameObject* gameObject, const Mesh& mesh, const Material& material) {
	if (gameObject) {
		// Create a new mesh with the specified material
		Mesh meshWithMaterial(mesh.getVertices(), mesh.getIndices(), material);
		m_meshRenderers[gameObject] = std::make_unique<MeshRenderer>(meshWithMaterial);
		BOTAPICA_LOG_INFO("Mesh with custom material added to GameObject: " + gameObject->GetName());
	}
}

void Scene::RemoveMeshFromGameObject(GameObject* gameObject) {
	auto it = m_meshRenderers.find(gameObject);
	if (it != m_meshRenderers.end()) {
		m_meshRenderers.erase(it);
		BOTAPICA_LOG_INFO("Mesh removed from GameObject");
	}
}

MeshRenderer* Scene::GetMeshRenderer(GameObject* gameObject) {
	auto it = m_meshRenderers.find(gameObject);
	return (it != m_meshRenderers.end()) ? it->second.get() : nullptr;
}

// Primitive Creation Helpers
GameObject* Scene::CreateCube(const String& name, const Vec3& position, float size, const Material& material) {
	GameObject* obj = CreateGameObject(name, position);
	Mesh cubeMesh = Primitives::createCube(material, size);
	AddMeshToGameObject(obj, cubeMesh);
	return obj;
}

GameObject* Scene::CreateSphere(const String& name, const Vec3& position, float radius, const Material& material) {
	GameObject* obj = CreateGameObject(name, position);
	Mesh sphereMesh = Primitives::createSphere(material, radius);
	AddMeshToGameObject(obj, sphereMesh);
	return obj;
}

GameObject* Scene::CreatePlane(const String& name, const Vec3& position, float width, float height, const Material& material) {
	GameObject* obj = CreateGameObject(name, position);
	Mesh planeMesh = Primitives::createPlane(material, width, height);
	AddMeshToGameObject(obj, planeMesh);
	return obj;
}

GameObject* Scene::CreateCylinder(const String& name, const Vec3& position, float radius, float height, const Material& material) {
	GameObject* obj = CreateGameObject(name, position);
	Mesh cylinderMesh = Primitives::createCylinder(material, radius, height);
	AddMeshToGameObject(obj, cylinderMesh);
	return obj;
}

// Simplified primitive creation methods (always use default material)
GameObject* Scene::CreateCubeSimple(const String& name, const Vec3& position, float size) {
	return CreateCube(name, position, size, Material::CreateDefaultMaterial());
}

GameObject* Scene::CreateSphereSimple(const String& name, const Vec3& position, float radius) {
	return CreateSphere(name, position, radius, Material::CreateDefaultMaterial());
}

GameObject* Scene::CreatePlaneSimple(const String& name, const Vec3& position, float width, float height) {
	return CreatePlane(name, position, width, height, Material::CreateDefaultMaterial());
}

GameObject* Scene::CreateCylinderSimple(const String& name, const Vec3& position, float radius, float height) {
	return CreateCylinder(name, position, radius, height, Material::CreateDefaultMaterial());
}

// Material management for existing objects
void Scene::SetObjectMaterial(GameObject* gameObject, const Material& material) {
	if (!gameObject) {
		BOTAPICA_LOG_WARN("Cannot set material on null GameObject");
		return;
	}
	
	MeshRenderer* renderer = GetMeshRenderer(gameObject);
	if (renderer) {
		// Get the current mesh data
		const Mesh& currentMesh = renderer->GetMesh();
		
		// Create a new mesh with the same geometry but new material
		Mesh newMesh(currentMesh.getVertices(), currentMesh.getIndices(), material);
		
		// Replace the mesh in the renderer
		renderer->SetMesh(newMesh);
		
		BOTAPICA_LOG_INFO("Material updated for GameObject: " + gameObject->GetName());
	} else {
		BOTAPICA_LOG_WARN("GameObject " + gameObject->GetName() + " has no mesh renderer - cannot set material");
	}
}

Material Scene::GetObjectMaterial(GameObject* gameObject) const {
	if (!gameObject) {
		BOTAPICA_LOG_WARN("Cannot get material from null GameObject");
		return Material::CreateDefaultMaterial();
	}
	
	const MeshRenderer* renderer = const_cast<Scene*>(this)->GetMeshRenderer(gameObject);
	if (renderer) {
		return renderer->GetMesh().getMaterial();
	} else {
		BOTAPICA_LOG_WARN("GameObject " + gameObject->GetName() + " has no mesh renderer - returning default material");
		return Material::CreateDefaultMaterial();
	}
}

void Scene::SetObjectMaterialToDefault(GameObject* gameObject) {
	SetObjectMaterial(gameObject, Material::CreateDefaultMaterial());
}

// Material preset helpers
void Scene::SetObjectMaterialToMetal(GameObject* gameObject) {
	SetObjectMaterial(gameObject, Material::CreateMetalMaterial());
}

void Scene::SetObjectMaterialToPlastic(GameObject* gameObject) {
	SetObjectMaterial(gameObject, Material::CreatePlasticMaterial());
}

void Scene::SetObjectMaterialToGlass(GameObject* gameObject) {
	SetObjectMaterial(gameObject, Material::CreateGlassMaterial());
}

void Scene::SetObjectMaterialColor(GameObject* gameObject, const Vec3& color) {
	Material colorMaterial = Material::CreateDefaultMaterial();
	colorMaterial.SetDiffuse(color);
	// Adjust ambient to be slightly darker version of the diffuse color
	colorMaterial.SetAmbient(color * 0.3f);
	SetObjectMaterial(gameObject, colorMaterial);
}

// Utility
void Scene::ResetScene() {
	// Clear all mesh renderers
	m_meshRenderers.clear();
	
	// Clear all lights
	m_lights.clear();
	
	// Destroy all GameObjects
	ObjectManager::DestroyAllObjects();
	ObjectManager::ProcessDestroyQueue();
	
	// Reset camera to default
	CreateDefaultCamera();
	
	BOTAPICA_LOG_INFO("Scene reset");
}

void Scene::FrameAll() {
	if (!m_camera || m_meshRenderers.empty()) {
		return;
	}
	
	// Calculate bounding box of all objects
	bool first = true;
	Vec3 minBounds, maxBounds;
	
	for (const auto& pair : m_meshRenderers) {
		GameObject* obj = pair.first;
		if (obj && obj->IsActive()) {
			Vec3 pos = obj->GetPosition();
			Vec3 scale = obj->GetScale();
			
			// Simple bounding estimation (could be improved)
			Vec3 objMin = pos - scale;
			Vec3 objMax = pos + scale;
			
			if (first) {
				minBounds = objMin;
				maxBounds = objMax;
				first = false;
			} else {
				minBounds = glm::min(minBounds, objMin);
				maxBounds = glm::max(maxBounds, objMax);
			}
		}
	}
	
	if (!first) {
		Vec3 center = (minBounds + maxBounds) * 0.5f;
		float radius = glm::length(maxBounds - minBounds) * 0.5f;
		m_camera->FrameAll(center, radius);
	}
}

// Private Implementation
void Scene::SetupDefaultShader() {
	// Load default vertex and fragment shaders
	const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoord = aTexCoord;
	
	gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

	const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;

void main() {
	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
)";

	// Create and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// Check for compilation errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Vertex shader compilation failed: " + String(infoLog));
		return;
	}
	
	// Create and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Fragment shader compilation failed: " + String(infoLog));
		glDeleteShader(vertexShader);
		return;
	}
	
	// Create shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);
	
	// Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Shader program linking failed: " + String(infoLog));
		m_shaderProgram = 0;
	}
	
	// Clean up shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	if (m_shaderProgram != 0) {
		BOTAPICA_LOG_INFO("Default shader created successfully");
	}
}

void Scene::ApplyRenderingSettings() {
	// Depth testing
	if (m_depthTestEnabled) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
	
	// Backface culling
	if (m_backfaceCullingEnabled) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else {
		glDisable(GL_CULL_FACE);
	}
	
	// Wireframe mode
	if (m_wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Scene::RenderMeshes() {
	if (!m_camera) return;
	
	Mat4 viewMatrix = m_camera->GetViewMatrix();
	Mat4 projectionMatrix = m_camera->GetProjectionMatrix();
	
	// Set view and projection matrices
	GLint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(m_shaderProgram, "projection");
	GLint viewPosLoc = glGetUniformLocation(m_shaderProgram, "viewPos");
	
	if (viewLoc != -1) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	if (projLoc != -1) glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	if (viewPosLoc != -1) glUniform3fv(viewPosLoc, 1, glm::value_ptr(m_camera->GetPosition()));
	
	// Render each mesh
	for (const auto& pair : m_meshRenderers) {
		GameObject* obj = pair.first;
		MeshRenderer* renderer = pair.second.get();
		
		if (obj && renderer && obj->IsActive() && renderer->IsEnabled()) {
			// Set model matrix
			Mat4 modelMatrix = obj->GetTransformMatrix();
			GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
			if (modelLoc != -1) {
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			}
			
			// Set material properties
			const Material& material = renderer->GetMesh().getMaterial();
			SetMaterialUniforms(material);
			
			// Render the mesh
			renderer->Render(modelMatrix, viewMatrix, projectionMatrix, m_lights);
		}
	}
}

void Scene::SetupLighting() {
	// Set default lighting if no lights exist
	if (m_lights.empty()) {
		GLint lightColorLoc = glGetUniformLocation(m_shaderProgram, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(m_shaderProgram, "lightPos");
		
		if (lightColorLoc != -1) glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		if (lightPosLoc != -1) glUniform3f(lightPosLoc, 2.0f, 4.0f, 2.0f);
		return;
	}
	
	// Use the first active light for now (could be extended for multiple lights)
	Light* activeLight = nullptr;
	for (Light* light : m_lights) {
		if (light && light->IsActive()) {
			activeLight = light;
			break;
		}
	}
	
	if (activeLight) {
		GLint lightColorLoc = glGetUniformLocation(m_shaderProgram, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(m_shaderProgram, "lightPos");
		
		Vec3 lightColor = activeLight->GetColor() * activeLight->GetIntensity();
		if (lightColorLoc != -1) glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
		if (lightPosLoc != -1) glUniform3fv(lightPosLoc, 1, glm::value_ptr(activeLight->GetPosition()));
	}
}

void Scene::SetMaterialUniforms(const Material& material) {
	GLint objectColorLoc = glGetUniformLocation(m_shaderProgram, "objectColor");
	if (objectColorLoc != -1) {
		glUniform3fv(objectColorLoc, 1, glm::value_ptr(material.GetDiffuse()));
	}
}

void Scene::CleanupDestroyedObjects() {
	// Remove mesh renderers for destroyed objects
	auto it = m_meshRenderers.begin();
	while (it != m_meshRenderers.end()) {
		GameObject* obj = it->first;
		if (!obj || obj->IsDestroyed()) {
			it = m_meshRenderers.erase(it);
		} else {
			++it;
		}
	}

	// Remove destroyed lights
	auto lightIt = m_lights.begin();
	while (lightIt != m_lights.end()) {
		Light* light = *lightIt;
		if (!light || light->IsDestroyed()) {
			lightIt = m_lights.erase(lightIt);
		} else {
			++lightIt;
		}
	}

	// Process the global destroy queue
	ObjectManager::ProcessDestroyQueue();
}
