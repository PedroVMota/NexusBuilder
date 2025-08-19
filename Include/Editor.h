#pragma once

#include "Viewport.h"
#include "Console.h"

class Editor {
public:


	// Editor(const Engine *engine);
	void BeginDockspace();
	void EndDockspace();
	void Render();
	void RenderMenuBar();
	
	// Global console access
	static Console* GetConsole() { return s_console; }
	static void SetConsole(Console* console) { s_console = console; }
	Editor();
	~Editor();
	
private:
	bool m_dockspaceOpen = true;
	Viewport m_viewport;
	Console m_console;
	bool m_showFileExplorer = true;
	bool m_showConsole = true;
	bool m_showViewport = true;
	
	static Console* s_console;



};
