#include "main.hpp"

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
		
		// Create the dockspace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
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
	ImGui::ShowDemoWindow();
	ImGui::Begin("TEST IMGUI WINDOW");
	ImGui::Text("test text");
	ImGui::End();


	fileExplorer.Render();

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
