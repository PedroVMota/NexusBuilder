#include "GUIWorkspace.h"
#include "Interfaces/GUIView.h"
#include "imgui.h"
#include "main.h"

GUIWorkspace::GUIWorkspace() : GUIView() {
    // Empty constructor - no ImGui operations here
}

GUIWorkspace::GUIWorkspace(const GUIWorkspace &other) : GUIView() {
    // Copy constructor
}

GUIWorkspace &GUIWorkspace::operator=(const GUIWorkspace &other) {
    // Assignment operator
    return *this;
}

GUIWorkspace::~GUIWorkspace() {
    // Destructor
}

GUIView *GUIWorkspace::create() {
    return new GUIWorkspace();
}

void GUIWorkspace::start() {
    BOTAPICA_LOG_INFO("GUIWorkspace starting");
    // No need to do anything special here since ImGui is already initialized in Engine
}

void GUIWorkspace::render() const {
    BOTAPICA_LOG_INFO("GUIWorkspace View Rendering");
    
    // Get IO each frame
    ImGuiIO& io = ImGui::GetIO();
    
    // Set background to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Create dockspace if docking is enabled
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        
        // Set background to black
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
        // Begin the dockspace window - THIS IS CRITICAL!
        ImGui::Begin("DockSpace", nullptr, window_flags);
        
        // Pop style vars after Begin
        ImGui::PopStyleVar(3);
        
        // Create the dockspace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        
        // End the window - THIS IS CRITICAL!
        ImGui::End();
        
        // Pop style color
        ImGui::PopStyleColor();
    }
}

void GUIWorkspace::destroy() {
    BOTAPICA_LOG_INFO("GUIWorkspace destroying");
    // No special cleanup needed here
}
