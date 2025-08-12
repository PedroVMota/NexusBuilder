#pragma once

#include "imgui.h"
#include "main.h"
#include <filesystem>
#include <vector>
#include <string>
#include <mutex>

using String = std::string;

// Forward declarations
class Console;

class Viewport {
public:
	Viewport();
	~Viewport();
	void Render();
	void Resize(int width, int height);
	unsigned int GetFramebufferTexture() const { return m_colorTexture; }
	
private:
	void CreateFramebuffer();
	void DeleteFramebuffer();
	
	unsigned int m_framebuffer = 0;
	unsigned int m_colorTexture = 0;
	unsigned int m_depthRenderbuffer = 0;
	int m_width = 800;
	int m_height = 600;
	bool m_framebufferValid = false;
};

class Console {
public:
	Console();
	~Console();
	void Render();
	void AddLog(const char* fmt, ...);
	void ClearLog();

private:
	void ExecCommand(const char* command_line);
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
	int TextEditCallback(ImGuiInputTextCallbackData* data);
	
	// String utils
	static int Stricmp(const char* s1, const char* s2);
	static int Strnicmp(const char* s1, const char* s2, int n);
	static char* Strdup(const char* s);
	static void Strtrim(char* s);
	
	ImVector<char*> m_items;
	ImVector<const char*> m_commands;
	ImVector<char*> m_history;
	char m_inputBuf[256];
	int m_historyPos;
	ImGuiTextFilter m_filter;
	bool m_autoScroll;
	bool m_scrollToBottom;
};

class Editor {
public:
	void BeginDockspace();
	void EndDockspace();
	void Render();
	void RenderMenuBar();
	
private:
	bool m_dockspaceOpen = true;
	Viewport m_viewport;
	Console m_console;
	bool m_showFileExplorer = true;
	bool m_showConsole = true;
	bool m_showViewport = true;
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