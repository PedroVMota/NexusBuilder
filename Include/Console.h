#pragma once

#include "imgui.h"

class Console {
public:
	Console();
	~Console();
	void Render();
	void AddLog(const char* fmt, ...);
	void ClearLog();
	void ClearHistory();

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