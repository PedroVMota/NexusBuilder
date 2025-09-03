#pragma once 

#include "main.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>


class Project {
public:
	std::string projectName;
	std::string path;
	std::string version;

	Project(const std::string& name, const std::string& path, const std::string& version)
		: projectName(name), path(path), version(version) {
	}
};

class Workspace {
private: 
	std::string _config_file;
	std::string _workspacelist_file;
	std::string getDataFolder(const char *) const;
	std::vector<Project> _projects; 

public:
	Workspace(const char*);
	~Workspace();

	static std::vector<std::string> getLines(const char* file);



	void loadWorkspace();
	void closeWorkspace();

	const std::vector<Project>& getProjects() const;

};
