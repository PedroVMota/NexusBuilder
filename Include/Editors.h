#pragma once
#include "imgui.h"
#include "main.hpp"
#include <filesystem>
#include <vector>
#include <string>

using String = std::string;


class Editor {
public:
	void BeginDockspace();
	void EndDockspace();
	void Render();
	
private:
	bool m_dockspaceOpen = true;
};


class FileExplorer {
public:
	FileExplorer();
	void Render();
	void SetCurrentPath(const String &path);

private:
	void RefreshDirectory();
	void RenderDirectoryTree();
	void RenderFileList();

	std::filesystem::path m_currPath;
	std::vector<std::filesystem::directory_entry> m_currFiles;
	std::vector<std::filesystem::directory_entry> m_currDirectory;
	String m_selectedFile;
	bool m_showHiddenFiles = false;
};