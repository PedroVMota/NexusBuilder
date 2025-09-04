#include "GUIWorkspace.h"
#include "Interfaces/GUIView.h"
#include "imgui.h"
#include "main.h"

void GUIWorkspace::renderHeaderSection()
{
 
    if (ImGui::BeginMenuBar()) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
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
        ImGui::PopStyleVar(2);

        ImGui::EndMenuBar();

    }
}

void GUIWorkspace::renderWorkspaceSection(const std::vector<Project> &projects)
{
    


    if (projects.size() > 0) {
        ImGui::Text("Recent Projects");
        ImGui::Separator();
        for (size_t i = 0; i < projects.size(); i++) {
            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Selectable(projects[i].projectName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    // TODO OPEN THE PROJECT
                }
            }
            ImGui::Text("  %s", projects[i].path.c_str());
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", projects[i].version.c_str());
            ImGui::Spacing();
            ImGui::PopID();
        }
        ImGui::NextColumn();
    }
}

void GUIWorkspace::renderTemplateSection()
{
}

void GUIWorkspace::renderButtonSection()
{
    // Right panel - Actions and Info
    ImGui::Text("Quick Actions");
    ImGui::Separator();

    ImVec2 size = ImGui::GetContentRegionAvail();
    if (ImGui::Button("New Project", ImVec2(size[0], 30))) {
        // Show new project dialog
    }

    if (ImGui::Button("Open Project", ImVec2(size[0], 30))) {
        // Show file browser
    }

    if (ImGui::Button("Clone Repository", ImVec2(size[0], 30))) {
        // Show git clone dialog
    }
}

GUIWorkspace::GUIWorkspace() : GUIView() {
    // Empty constructor - no ImGui operations here
    this->_spaceManager = std::make_shared<Workspace>(PROCESS_NAME);
    this->_spaceManager->loadWorkspace();

    std::vector<Project> _project = this->_spaceManager->getProjects();
    for (const auto& project : _project) {
		std::cout << "Memory Loss: " << project.projectName << " at " << project.path << " version " << project.version << std::endl;
    }
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

void GUIWorkspace::render() {
    //BOTAPICA_LOG_INFO("GUIWorkspace View Rendering");
    
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
        this->renderHeaderSection();
        // Get available space

        const std::vector<Project>& projects = this->_spaceManager->getProjects();
        ImVec2 available_size = ImGui::GetContentRegionAvail();
        // Main workspace content with styling
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));

        ImGui::Columns(2, "WorkspaceColumns", false);
        ImGui::SetColumnWidth(0, available_size.x * 0.8f);  // 30% for recent projects
        ImGui::SetColumnWidth(1, available_size.x * 0.4f);  // 40% for templates
        if(!projects.size())
        {
            ImGui::Text("No Recent Projects");
            ImGui::NextColumn();
        }
        else
        {
            this->renderWorkspaceSection(projects);
        }
        
        this->renderButtonSection();
        
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(5);
        ImGui::End();
        ImGui::PopStyleColor();
    }
}

void GUIWorkspace::destroy() {
    BOTAPICA_LOG_INFO("GUIWorkspace destroying");
    // No special cleanup needed here
}

