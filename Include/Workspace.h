#pragma once 

#include "main.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>



struct FileEnviroment {
	std::filesystem::path filepath;
	std::shared_ptr<std::ofstream> file;
	std::shared_ptr<std::string> content;

	FileEnviroment &operator=(const FileEnviroment &o){
		this->filepath = o.filepath;
		this->file = o.file;
		this->content = o.content;
		return *this;
	}

	FileEnviroment() {}
	~FileEnviroment() {
		BOTAPICA_LOG_INFO("Cloding File Enviroment");
		this->filepath.clear();
	}
};

class Workspace {

private: 
	std::map<std::string, FileEnviroment> _opened_files;

	std::string _config_file;
	std::string _workspacelist_file;
	std::string getDataFolder(const char *) const;


public:
	Workspace(const char *);
	~Workspace();

	Workspace(const Workspace &) = delete;
	Workspace &operator=(const Workspace &) = delete;


	void loadWorkspace();
	void closeWorkspace();

	FileEnviroment load(const std::string &);
};
