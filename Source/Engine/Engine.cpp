#include "Engine.h"
#include "Screens/GUIWorkspace.h"


Engine::Engine() : window(nullptr), editor(nullptr), state(RUNNING)  {
	
}

Engine::~Engine() {
	Shutdown();
}

int Engine::Initialize() {
	BOTAPICA_LOG_INFO("Initializing Botapica Engine...");
	
	if (!InitializeGLFW()) {
		return -1;
	}
	
	if (!CreateWindow()) {
		return -1;
	}
	
	if (!InitializeOpenGL()) {
		return -1;
	}
	
	LogGPUInfo();
	InitializeImGui();
	
	// Editor *editor = new Editor();
	//
	this->editor = GUIWorkspace::create();
	
	BOTAPICA_LOG_INFO("Engine initialized successfully");
	return 0;
}

int Engine::Run() {
	if (!window || !editor) {
		BOTAPICA_LOG_ERROR("Engine not properly initialized");
		return -1;
	}
	
	MainLoop();
	return 0;
}

void Engine::Shutdown() {
	if (editor) {
		delete editor;
		editor = nullptr;
	}
	
	// Cleanup ImGui before destroying window
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	if (window) {
		glfwDestroyWindow(window);
		window = nullptr;
	}
	
	glfwTerminate();
	BOTAPICA_LOG_INFO("Engine shutdown complete.");
}

bool Engine::InitializeGLFW() {
	if (!glfwInit()) {
		BOTAPICA_LOG_ERROR("Failed to initialize GLFW!");
		return false;
	}
	
	SetupGLFWHints();
	return true;
}

bool Engine::CreateWindow() {
	window = glfwCreateWindow(WIDHT, HEIGHT, "3D Engine", nullptr, nullptr);
	
	if (!window) {
		BOTAPICA_LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	
	glfwSetErrorCallback([](int error, const char* description) {
		BOTAPICA_LOG_ERROR(std::string("GLFW Error ") + std::to_string(error) + ": " + description);
	});
	
	glfwMakeContextCurrent(window);
	return true;
}

bool Engine::InitializeOpenGL() {
	if (glewInit() != GLEW_OK) {
		BOTAPICA_LOG_ERROR("Failed to initialize GLEW");
		glfwTerminate();
		return false;
	}
	
	glEnable(GL_DEPTH_TEST);
	return true;
}

void Engine::InitializeImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Engine::LogGPUInfo() {
	BOTAPICA_LOG_INFO(String("OpenGL Version: ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	BOTAPICA_LOG_INFO(String("GPU: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	BOTAPICA_LOG_INFO(String("OpenGL Vendor: ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)));

	String renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	if (renderer.find("llvmpipe") != std::string::npos) 
		BOTAPICA_LOG_INFO("Using software rendering (llvmpipe). GPU acceleration not available.");
	else if (renderer.find("NVIDIA") != std::string::npos)
		BOTAPICA_LOG_INFO("NVIDIA GPU acceleration detected!");
	else
		BOTAPICA_LOG_INFO("Hardware-accelerated rendering detected.");
}

void Engine::SetupGLFWHints() {
#ifdef PLATFORM_MACOS
	BOTAPICA_LOG_INFO("Setting up GLFW hints for macOS...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif defined(PLATFORM_WINDOWS)
	BOTAPICA_LOG_INFO("Setting up GLFW hints for Windows...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif defined(PLATFORM_LINUX)
	BOTAPICA_LOG_INFO("Setting up GLFW hints for Linux...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else
	BOTAPICA_LOG_INFO("Setting up GLFW hints for unknown platform...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

#ifdef BOTAPICA_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
}













bool Engine::renderWorkspaceManager(){
	this->editor->render();
	return false;
}

void Engine::MainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if(this->state == CHOOSING_WORKSPACE){
			this->renderWorkspaceManager();
		}
		else{
			editor->render();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}
