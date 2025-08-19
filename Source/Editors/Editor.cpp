#include "Editor.h"
#include "FileExplorer.h"
#include "main.h"
#include <cstdlib>

// Global logging function pointer definition
void (*g_consoleLogFunc)(const char* level, const char* message) = nullptr;

// Static member definition
Console* Editor::s_console = nullptr;



Editor::Editor() {}
Editor::~Editor() {}

// Console logging wrapper function
void ConsoleLogWrapper(const char* level, const char* message) {
	if (Editor::GetConsole()) {
		Editor::GetConsole()->AddLog("[%s] %s", level, message);
	}
}

void Editor::BeginDockspace()
{
	// Enable docking
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		// Create a fullscreen window for the dockspace
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		
		ImGui::Begin("DockSpace", &m_dockspaceOpen, window_flags);
		ImGui::PopStyleVar(3);

		RenderMenuBar();
		// Create the dockspace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}
}

void Editor::RenderMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N")) {
				m_console.AddLog("New button pressed");
			}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				m_console.AddLog("Open button pressed");
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				m_console.AddLog("Save button pressed");
			}
			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {
				m_console.AddLog("Save as button pressed");
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				m_console.AddLog("Exit button pressed");
				exit(EXIT_SUCCESS);
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
				// Handle undo
			}
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
				// Handle redo
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X")) {
				// Handle cut
			}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {
				// Handle copy
			}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {
				// Handle paste
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("File Explorer", nullptr, &m_showFileExplorer);
			ImGui::MenuItem("Console", nullptr, &m_showConsole);
			ImGui::MenuItem("Viewport", nullptr, &m_showViewport);
			ImGui::Separator();
			if (ImGui::MenuItem("Reset Layout")) {
				m_showConsole = true;
				m_showFileExplorer = true;
				m_showViewport = true;
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Settings")) {
				// Open settings window
			}
			if (ImGui::MenuItem("Console", "~")) {
				m_showConsole = !m_showConsole;
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) {
				// Show about dialog
			}
			if (ImGui::MenuItem("Documentation")) {
				// Open documentation
			}
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}
}

void Editor::EndDockspace()
{
	ImGui::End();
}

void Editor::Render()
{
	static FileExplorer fileExplorer;
	
	// Set up global console access on first run
	if (s_console == nullptr) {
		s_console = &m_console;
		g_consoleLogFunc = ConsoleLogWrapper;  // Set global logging function
	}
	
	BeginDockspace();
	
	if (m_showFileExplorer)
		fileExplorer.Render();
	
	if (m_showViewport)
		m_viewport.Render();
	
	if (m_showConsole)
		m_console.Render();

	EndDockspace();
}
