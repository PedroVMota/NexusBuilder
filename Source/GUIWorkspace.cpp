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
        
        // Menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                    // Handle new file
                }
                if (ImGui::MenuItem("Open")) {
                    // Handle open file
                }
                if (ImGui::MenuItem("Save")) {
                    // Handle save file
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    // Handle exit
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Mode One")) {
                    // Switch to mode one
                }
                if (ImGui::MenuItem("Mode Two")) {
                    // Switch to mode two
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        // Get available space
        ImVec2 available_size = ImGui::GetContentRegionAvail();
        
        // Main workspace content with styling
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        
        ImGui::Columns(3, "WorkspaceColumns", false);
        ImGui::SetColumnWidth(0, available_size.x * 0.3f);  // 30% for recent projects
        ImGui::SetColumnWidth(1, available_size.x * 0.4f);  // 40% for templates
        
        // Left panel - Recent Projects
        ImGui::Text("Recent Projects");
        ImGui::Separator();
        
        // Project list with icons and details
        const char* projects[] = {"MyGameEngine", "WebApp_2024", "AI_ChatBot", "MobileGame", "DataAnalyzer"};
        const char* paths[] = {"/Users/dev/MyGameEngine", "/Users/dev/WebApp_2024", "/Users/dev/AI_ChatBot", "/Users/dev/MobileGame", "/Users/dev/DataAnalyzer"};
        const char* dates[] = {"2 days ago", "1 week ago", "2 weeks ago", "1 month ago", "2 months ago"};
        
        for(int i = 0; i < 5; i++) {
            ImGui::PushID(i);
            if(ImGui::Selectable(projects[i], false, ImGuiSelectableFlags_AllowDoubleClick)) {
                if(ImGui::IsMouseDoubleClicked(0)) {
                    // Open project
                }
            }
            ImGui::Text("  %s", paths[i]);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", dates[i]);
            ImGui::Spacing();
            ImGui::PopID();
        }
        
        ImGui::NextColumn();
        
        // Middle panel - Project Templates
        ImGui::Text("Create New Project");
        ImGui::Separator();
        
        // Template categories
        if(ImGui::TreeNodeEx("Game Development", ImGuiTreeNodeFlags_DefaultOpen)) {
            if(ImGui::Selectable("2D Platformer")) { /* Create 2D game */ }
            if(ImGui::Selectable("3D Adventure")) { /* Create 3D game */ }
            if(ImGui::Selectable("Puzzle Game")) { /* Create puzzle game */ }
            ImGui::TreePop();
        }
        
        if(ImGui::TreeNodeEx("Applications", ImGuiTreeNodeFlags_DefaultOpen)) {
            if(ImGui::Selectable("Desktop App")) { /* Create desktop app */ }
            if(ImGui::Selectable("Web Application")) { /* Create web app */ }
            if(ImGui::Selectable("Mobile App")) { /* Create mobile app */ }
            ImGui::TreePop();
        }
        
        if(ImGui::TreeNodeEx("Tools & Utilities", ImGuiTreeNodeFlags_DefaultOpen)) {
            if(ImGui::Selectable("Console Tool")) { /* Create console tool */ }
            if(ImGui::Selectable("Library/Framework")) { /* Create library */ }
            if(ImGui::Selectable("Plugin/Extension")) { /* Create plugin */ }
            ImGui::TreePop();
        }
        
        ImGui::NextColumn();
        
        // Right panel - Actions and Info
        ImGui::Text("Quick Actions");
        ImGui::Separator();
        
        if(ImGui::Button("New Project", ImVec2(120, 30))) {
            // Show new project dialog
        }
        
        if(ImGui::Button("Open Project", ImVec2(120, 30))) {
            // Show file browser
        }
        
        if(ImGui::Button("Clone Repository", ImVec2(120, 30))) {
            // Show git clone dialog
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Recent activity section
        ImGui::Text("Recent Activity");
        ImGui::Separator();
        ImGui::BulletText("Opened MyGameEngine");
        ImGui::BulletText("Created new branch 'feature-ui'");
        ImGui::BulletText("Committed 5 files");
        ImGui::BulletText("Built project successfully");
        
        ImGui::Columns(1);
        
        // Pop workspace styling
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
        
        // Pop style vars after Begin
        ImGui::PopStyleVar(3);
        
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
