#include "main.h"

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
	BeginDockspace();
	
	if (m_showFileExplorer)
		fileExplorer.Render();
	
	if (m_showViewport)
		m_viewport.Render();
	
	if (m_showConsole)
		m_console.Render();

	EndDockspace();
}

FileExplorer::FileExplorer()
{
	m_currPath = std::filesystem::current_path();
	RefreshDirectory();
};

void FileExplorer::Render()
{
	ImGui::Begin("File Explorer");

	ImGui::Text("Current Path: %s", m_currPath.string().c_str());
	ImGui::Separator();

	if (ImGui::Button("..") && m_currPath.has_parent_path())
	{
		SetCurrentPath(m_currPath.parent_path().string());
	}
	ImGui::SameLine();
	ImGui::Checkbox("Show Hidden files", &m_showHiddenFiles);
	ImGui::Separator();

	RenderFileList();
	
	ImGui::End();
};

void FileExplorer::SetCurrentPath(const String &path)
{
		try
	{
		std::filesystem::path newPath(path);
		
		// Check if path exists and is accessible
		if (!std::filesystem::exists(newPath))
		{
			BOTAPICA_LOG_ERROR("Path does not exist: " + path);
			return;
		}
		
		if (!std::filesystem::is_directory(newPath))
		{
			BOTAPICA_LOG_ERROR("Path is not a directory: " + path);
			return;
		}
		std::error_code ec;
		auto iter = std::filesystem::directory_iterator(newPath, ec);
		if (ec)
		{
			BOTAPICA_LOG_ERROR("Cannot access directory (permission denied?): " + path + " - " + ec.message());
			return;
		}

		BOTAPICA_LOG_INFO("Successfully navigating to: " + path);
		m_currPath = std::filesystem::path(path);
		RefreshDirectory();
	}
	catch(const std::filesystem::filesystem_error& e)
	{
		BOTAPICA_LOG_ERROR("SetCurrentPath error: " + String(e.what()));
	}
	catch(const std::exception& e)
	{
		BOTAPICA_LOG_ERROR("SetCurrentPath unexpected error: " + String(e.what()));
	}
};

void FileExplorer::RefreshDirectory()
{
	m_currFiles.clear();
	m_currDirectory.clear();

	try
	{
		if (!std::filesystem::exists(m_currPath) || !std::filesystem::is_directory(m_currPath))
		{
			BOTAPICA_LOG_ERROR("Invalid current path, resetting to working directory");
			m_currPath = std::filesystem::current_path();
		}
		
		BOTAPICA_LOG_INFO("Refreshing directory: " + m_currPath.string());
		
		std::error_code ec;
		std::filesystem::directory_iterator iter(m_currPath, ec);
		if (ec)
		{
			BOTAPICA_LOG_ERROR("Cannot create directory iterator: " + ec.message());
			return;
		}
		
		for (const auto& entry : iter)
		{
			try
			{
				std::string filename = entry.path().filename().string();
				BOTAPICA_LOG_INFO("Processing entry: " + filename);
				
				if (!m_showHiddenFiles && !filename.empty() && filename[0] == '.')
					continue;
				
				std::error_code entry_ec;
				if (entry.is_directory(entry_ec) && !entry_ec)
				{
					m_currDirectory.push_back(entry);
					BOTAPICA_LOG_INFO("Added directory: " + filename);
				}
				else if (entry.is_regular_file(entry_ec) && !entry_ec)
				{
					m_currFiles.push_back(entry);
					BOTAPICA_LOG_INFO("Added file: " + filename);
				}
			}
			catch (const std::exception& e)
			{
				BOTAPICA_LOG_ERROR("Error processing directory entry: " + std::string(e.what()));
				continue;
			}
		}
		
		// Sort directories alphabetically
		std::sort(m_currDirectory.begin(), m_currDirectory.end(), 
			[](const auto &a, const auto &b)
			{
				try {
					return a.path().filename().string() < b.path().filename().string();
				} catch (...) {
					return false;
				}
			});
		
		// Sort files alphabetically  
		std::sort(m_currFiles.begin(), m_currFiles.end(), 
			[](const auto &a, const auto &b)
			{
				try {
					return a.path().filename().string() < b.path().filename().string();
				} catch (...) {
					return false;
				}
			});
			
		BOTAPICA_LOG_INFO("Refreshed directory: " + std::to_string(m_currDirectory.size()) + " dirs, " + std::to_string(m_currFiles.size()) + " files");
	}
	catch(const std::filesystem::filesystem_error& e)
	{
		BOTAPICA_LOG_ERROR("Refresh directory filesystem error: " + std::string(e.what()));
		try {
			m_currPath = std::filesystem::current_path();
		} catch (...) {
			BOTAPICA_LOG_ERROR("Cannot access current directory");
		}
	}
	catch(const std::exception& e)
	{
		BOTAPICA_LOG_ERROR("Refresh directory unexpected error: " + std::string(e.what()));
	}
}

void FileExplorer::RenderFileList()
{
	for (size_t i = 0; i < m_currDirectory.size(); ++i)
	{
		try
		{
			const auto& dir = m_currDirectory[i];
			std::string filename = dir.path().filename().string();
			std::string displayName = "[DIR] " + filename;

			if (ImGui::Selectable(displayName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				// Simple double-click detection
				if (ImGui::IsMouseDoubleClicked(0))
				{
					try {
						std::string fullPath = dir.path().string();
						BOTAPICA_LOG_INFO("Double-clicked on: " + fullPath);
						SetCurrentPath(fullPath);
						break; // Exit the loop after navigation to prevent invalid access
					}
					catch (const std::exception& e)
					{
						BOTAPICA_LOG_ERROR("Failed to navigate to directory: " + std::string(e.what()));
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			BOTAPICA_LOG_ERROR("Error rendering directory entry at index " + std::to_string(i) + ": " + std::string(e.what()));
			continue;
		}
	}
	
	for (size_t i = 0; i < m_currFiles.size(); ++i)
	{
		try
		{
			const auto& file = m_currFiles[i];
			std::string filename = file.path().filename().string();
			std::string displayName = "[FILE] " + filename;
			
			bool isSelected = (m_selectedFile == filename);
			if (ImGui::Selectable(displayName.c_str(), isSelected))
			{
				m_selectedFile = filename;
			}
		}
		catch (const std::exception& e)
		{
			BOTAPICA_LOG_ERROR("Error rendering file entry at index " + std::to_string(i) + ": " + std::string(e.what()));
			continue;
		}
	}
}

Viewport::Viewport()
{
	CreateFramebuffer();
}

Viewport::~Viewport()
{
	DeleteFramebuffer();
}

void Viewport::CreateFramebuffer()
{
	// Delete existing framebuffer if it exists
	DeleteFramebuffer();

	// Generate framebuffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	
	// Create color texture
	glGenTextures(1, &m_colorTexture);
	glBindTexture(GL_TEXTURE_2D, m_colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

	// Create depth renderbuffer
	glGenRenderbuffers(1, &m_depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderbuffer);
	
	// Check framebuffer completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		m_framebufferValid = true;
		BOTAPICA_LOG_INFO("Viewport framebuffer created successfully");
	}
	else
	{
		m_framebufferValid = false;
		BOTAPICA_LOG_ERROR("Viewport framebuffer is not complete!");
	}
	
	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::DeleteFramebuffer()
{
	if (m_colorTexture != 0)
	{
		glDeleteTextures(1, &m_colorTexture);
		m_colorTexture = 0;
	}
	
	if (m_depthRenderbuffer != 0)
	{
		glDeleteRenderbuffers(1, &m_depthRenderbuffer);
		m_depthRenderbuffer = 0;
	}
	
	if (m_framebuffer != 0)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0;
	}
	
	m_framebufferValid = false;
}

void Viewport::Resize(int width, int height)
{
	if (width <= 0 || height <= 0) return;
	
	if (m_width != width || m_height != height)
	{
		m_width = width;
		m_height = height;
		CreateFramebuffer();
	}
}

void Viewport::Render()
{
	ImGui::Begin("Viewport");
	
	// Get available content region
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	
	// Resize framebuffer if needed
	if (contentRegion.x > 0 && contentRegion.y > 0)
	{
		Resize(static_cast<int>(contentRegion.x), static_cast<int>(contentRegion.y));
		
		if (m_framebufferValid)
		{
			// Bind our framebuffer for rendering
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glViewport(0, 0, m_width, m_height);
			
			// Clear the framebuffer
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// TODO: Render 3D scene here
			
			// Unbind framebuffer (back to default)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			// Display the rendered texture in ImGui
			ImGui::Image(
				reinterpret_cast<void*>(static_cast<intptr_t>(m_colorTexture)),
				contentRegion,
				ImVec2(0, 1), // UV coordinates flipped
				ImVec2(1, 0)
			);
		}
		else
		{
			ImGui::Text("Framebuffer error - cannot render viewport");
		}
	}
	else
	{
		ImGui::Text("Viewport too small");
	}
	ImGui::End();
}
