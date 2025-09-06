#include "GUIWorkspace.h"
#include "Interfaces/GUIView.h"
#include "imgui.h"
#include "main.h"


void GUIWorkspace::renderHeaderSection()
{

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
                std::cout << "Exit Button Being pressed\n";
                this->engine->setRuning(CLOSE);
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

}

void GUIWorkspace::renderWorkspaceSection(const std::vector<Project>& projects)
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
    }
}

void GUIWorkspace::renderTemplateSection()
{
}

void GUIWorkspace::renderButtonSection()
{
    // Right panel - Actions and Info
    ImGui::Text("Quick Actions");

    ImVec2 size = ImGui::GetContentRegionAvail();
    if (ImGui::Button("New Project", ImVec2(size.x, 30))) {
        // Show new project dialog
    }

    if (ImGui::Button("Open Project", ImVec2(size.x, 30))) {
        // Show file browser
    }

    if (ImGui::Button("Clone Repository", ImVec2(size.x, 30))) {
        // Show git clone dialog
    }
}

GUIWorkspace::GUIWorkspace(Engine* _engine_ptr) : GUIView() {
    this->_spaceManager = std::make_shared<Workspace>(PROCESS_NAME);
    this->_spaceManager->loadWorkspace();
    this->engine = _engine_ptr;

    std::vector<Project> _project = this->_spaceManager->getProjects();
}

GUIWorkspace::GUIWorkspace(const GUIWorkspace& other) : GUIView() {
    // Copy constructor
}

GUIWorkspace& GUIWorkspace::operator=(const GUIWorkspace& other) {
    // Assignment operator
    return *this;
}

GUIWorkspace::~GUIWorkspace() {
    // Destructor
}

GUIView* GUIWorkspace::create(Engine * _ptr) {
    return new GUIWorkspace(_ptr);
}

void GUIWorkspace::start() {
    BOTAPICA_LOG_INFO("GUIWorkspace starting");
}

void GUIWorkspace::render() {
    ImGuiIO& io = ImGui::GetIO();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);

        // Render menu bar first
        this->renderHeaderSection();

        // Get available space AFTER menu bar
        ImVec2 available_size = ImGui::GetContentRegionAvail();
        ImVec2 MainSpace = ImVec2(0, available_size.y * 0.8f);

        // Main content area (80% of height)
        if (ImGui::BeginChild("MainArea", MainSpace, false)) {
            ImGui::Columns(2, "mycolumns2", false);
            ImGui::SetColumnWidth(0, available_size.x * 0.6f);  // Fixed: don't exceed 100%
            ImGui::SetColumnWidth(1, available_size.x * 0.4f);

            // Projects column
            if (ImGui::BeginChild("ProjectsScrollArea", ImVec2(0, 0), false)) {
                if (this->_spaceManager->getProjects().size() > 0)
                    this->renderWorkspaceSection(this->_spaceManager->getProjects());
            }
            ImGui::EndChild();

            ImGui::NextColumn();
            this->renderButtonSection();
        }
        ImGui::EndChild();

        // Bottom area (remaining 20% of height) - your buttons go here
        // Add your bottom-snapped elements here

        if (ImGui::BeginChild("BottomArea", ImVec2(0, 0), false)) {
            ImVec2 AvailableSpace = ImGui::GetContentRegionAvail(); // Missing semicolon

            ImGui::Columns(3, "bottomcolumns", false);

            // Calculate button width for each column
            ImVec2 buttonSize = ImVec2(AvailableSpace.x / 3.0f - 10.0f, AvailableSpace.y); // Subtract padding

            ImGui::Button("Report Issue", buttonSize);
            ImGui::NextColumn(); // Move to next column
            ImGui::Button("Button 2", buttonSize);
            ImGui::NextColumn();
            ImGui::Button("Button 3", buttonSize);

            ImGui::Columns(1); // Reset columns
            ImGui::EndChild();
        }

        ImGui::PopStyleColor();
        ImGui::End();
    }
}

void GUIWorkspace::destroy() {
    BOTAPICA_LOG_INFO("GUIWorkspace destroying");
}