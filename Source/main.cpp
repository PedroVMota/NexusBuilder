#include "main.hpp"
#include "Editors.h"

void LogGPUInfo()
{
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

void SetupGLFWHints()
{
#ifdef PLATFORM_MACOS
	BOTAPICA_LOG_INFO("Setting up GLFW hints for macOS...");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
	BOTAPICA_LOG_INFO("Setting up GLFW hints for Windows/Linux...");
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

int main()
{
	BOTAPICA_LOG_INFO("Initializing Botapica Engine...");

	if (!glfwInit())
	{
		BOTAPICA_LOG_ERROR("Failed to initialize GLFW!");
		return -1;
	}

	SetupGLFWHints();

	GLFWwindow* window = glfwCreateWindow(WIDHT, HEIGHT, "3D Engine", nullptr, nullptr);

	if (!window)
	{
		BOTAPICA_LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwSetErrorCallback([](int error, const char* description)
	{
		BOTAPICA_LOG_ERROR(std::string("GLFW Error ") + std::to_string(error) + ": " + description);
	});

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		BOTAPICA_LOG_ERROR("Failed to initialize GLEW");
		glfwTerminate();
		return -1;
	}

	LogGPUInfo();

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	BOTAPICA_LOG_INFO("Engine initialized successfully");


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	Editor editor;
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Clear screen
		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		editor.Render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	BOTAPICA_LOG_INFO("Engine shutdown complete.");
	return (0);
}