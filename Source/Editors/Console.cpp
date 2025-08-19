#include "Console.h"
#include <cstdarg>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

Console::Console()
{
	memset(m_inputBuf, 0, sizeof(m_inputBuf));
	m_historyPos = -1;
	m_autoScroll = true;
	m_scrollToBottom = false;
	
	// Add available commands
	m_commands.push_back("help");
	m_commands.push_back("clear");
	m_commands.push_back("history");
	
	AddLog("Debug Console initialized");
	AddLog("Type 'help' for available commands");
}

Console::~Console()
{
	ClearLog();
	ClearHistory();
}

// String utilities
int Console::Stricmp(const char* s1, const char* s2)
{
	int d;
	while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; }
	return d;
}

int Console::Strnicmp(const char* s1, const char* s2, int n)
{
	int d = 0;
	while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; }
	return d;
}

char* Console::Strdup(const char* s)
{
	size_t len = strlen(s) + 1;
	void* buf = malloc(len);
	if (buf == nullptr) {
		return nullptr;
	}
	return (char*)memcpy(buf, s, len);
}

void Console::Strtrim(char* s)
{
	char* str_end = s + strlen(s);
	while (str_end > s && str_end[-1] == ' ') str_end--;
	*str_end = 0;
}

void Console::ClearLog()
{
	for (int i = 0; i < m_items.Size; i++)
		free(m_items[i]);
	m_items.clear();
}

void Console::ClearHistory()
{
	for (int i = 0; i < m_history.Size; i++)
		free(m_history[i]);
	m_history.clear();
	m_historyPos = -1;
}

void Console::AddLog(const char* fmt, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	buf[sizeof(buf) - 1] = 0;
	va_end(args);
	char* item = Strdup(buf);
	if (item != nullptr) {
		m_items.push_back(item);
	}
}

void Console::Render()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Debug Console"))
	{
		ImGui::End();
		return;
	}

	// Context menu
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Clear Console"))
			ClearLog();
		ImGui::EndPopup();
	}

	ImGui::TextWrapped("Debug Console - Real-time application logs and commands");
	ImGui::TextWrapped("Enter 'help' for available commands");

	// Control buttons - simplified
	if (ImGui::SmallButton("Add Debug Text"))  //* CHANGE FOR SOMETHING ELSE
	{ 
		AddLog("%d some text", m_items.Size); 
		AddLog("some more text"); 
		AddLog("display very important message here!"); 
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Add Debug Error")) 
	{ 
		AddLog("[error] something went wrong"); 
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear"))
	{ 
		ClearLog(); 
	}

	ImGui::Separator();

	// Options menu - simplified
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &m_autoScroll);
		ImGui::EndPopup();
	}

	ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	m_filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	ImGui::Separator();

	// Log display area
	const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear"))
				ClearLog();
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
		
		for (const char* item : m_items)
		{
			if (!m_filter.PassFilter(item))
				continue;

			// Simple color logic exactly like ImGui example
			ImVec4 color;
			bool has_color = false;
			if (strstr(item, "[error]")) { 
				color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); 
				has_color = true; 
			}
			else if (strncmp(item, "# ", 2) == 0) { 
				color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); 
				has_color = true; 
			}
			
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(item);
			if (has_color)
				ImGui::PopStyleColor();
		}

		if (m_scrollToBottom || (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);
		m_scrollToBottom = false;

		ImGui::PopStyleVar();
	}
	ImGui::EndChild();
	ImGui::Separator();

	// Command input
	bool reclaimFocus = false;
	ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue | 
								   ImGuiInputTextFlags_EscapeClearsAll | 
								   ImGuiInputTextFlags_CallbackCompletion | 
								   ImGuiInputTextFlags_CallbackHistory;
	
	if (ImGui::InputText("Input", m_inputBuf, sizeof(m_inputBuf), inputFlags, &TextEditCallbackStub, this))
	{
		char* s = m_inputBuf;
		Strtrim(s);
		if (s[0])
			ExecCommand(s);
		strcpy(s, "");
		reclaimFocus = true;
	}

	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
		ImGui::SetKeyboardFocusHere(-1);

	ImGui::End();
}

void Console::ExecCommand(const char* commandLine)
{
	AddLog("# %s", commandLine);

	m_historyPos = -1;
	for (int i = m_history.Size - 1; i >= 0; i--)
	{
		if (Stricmp(m_history[i], commandLine) == 0)
		{
			free(m_history[i]);
			m_history.erase(m_history.begin() + i);
			break;
		}
	}
	char* histItem = Strdup(commandLine);
	if (histItem != nullptr) {
		m_history.push_back(histItem);
	}

	if (Stricmp(commandLine, "clear") == 0)
	{
		ClearLog();
	}
	else if (Stricmp(commandLine, "help") == 0)
	{
		AddLog("Commands:");
		for (int i = 0; i < m_commands.Size; i++)
			AddLog("- %s", m_commands[i]);
	}
	else if (Stricmp(commandLine, "history") == 0)
	{
		int first = m_history.Size - 10;
		for (int i = first > 0 ? first : 0; i < m_history.Size; i++)
			AddLog("%3d: %s", i, m_history[i]);
	}
	else
	{
		AddLog("Unknown command: '%s'", commandLine);
	}

	m_scrollToBottom = true;
}

int Console::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	Console* console = (Console*)data->UserData;
	return console->TextEditCallback(data);
}

int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Command completion
			const char* wordEnd = data->Buf + data->CursorPos;
			const char* wordStart = wordEnd;
			while (wordStart > data->Buf)
			{
				const char c = wordStart[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				wordStart--;
			}

			ImVector<const char*> candidates;
			for (int i = 0; i < m_commands.Size; i++)
				if (Strnicmp(m_commands[i], wordStart, (int)(wordEnd - wordStart)) == 0)
					candidates.push_back(m_commands[i]);

			if (candidates.Size == 0)
			{
				AddLog("No match for \"%.*s\"!", (int)(wordEnd - wordStart), wordStart);
			}
			else if (candidates.Size == 1)
			{
				data->DeleteChars((int)(wordStart - data->Buf), (int)(wordEnd - wordStart));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				int matchLen = (int)(wordEnd - wordStart);
				for (;;)
				{
					int c = 0;
					bool allCandidatesMatch = true;
					for (int i = 0; i < candidates.Size && allCandidatesMatch; i++)
						if (i == 0)
							c = tolower(candidates[i][matchLen]);
						else if (c == 0 || c != tolower(candidates[i][matchLen]))
							allCandidatesMatch = false;
					if (!allCandidatesMatch)
						break;
					matchLen++;
				}

				if (matchLen > 0)
				{
					data->DeleteChars((int)(wordStart - data->Buf), (int)(wordEnd - wordStart));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + matchLen);
				}

				AddLog("Possible matches:");
				for (int i = 0; i < candidates.Size; i++)
					AddLog("- %s", candidates[i]);
			}
			break;
		}
	case ImGuiInputTextFlags_CallbackHistory:
		{
			const int prevHistoryPos = m_historyPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (m_historyPos == -1)
					m_historyPos = m_history.Size - 1;
				else if (m_historyPos > 0)
					m_historyPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (m_historyPos != -1)
					if (++m_historyPos >= m_history.Size)
						m_historyPos = -1;
			}

			if (prevHistoryPos != m_historyPos)
			{
				const char* historyStr = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, historyStr);
			}
		}
	}
	return 0;
}
