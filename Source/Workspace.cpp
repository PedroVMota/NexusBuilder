#include "Workspace.h"
#include "main.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace fs = std::filesystem;


const std::vector<Project>& Workspace::getProjects() const { return this->_projects; }


std::string Workspace::getDataFolder(const char *_process) const {


#ifndef PLATFORM_WINDOWS
    const char* lastSlash = strrchr(_process, '/');
    std::string data_home_folder = getenv("XDG_DATA_HOME");
    if (!lastSlash)
        data_home_folder = data_home_folder + "/" + _process + "/";
    else
        data_home_folder = data_home_folder + lastSlash + "/";
#else
    const char* appdata = getenv("APPDATA");
    BOTAPICA_LOG_WARN("AppData: " << (appdata ? appdata : "Not Set"));

    std::string data_home_folder;
    if (appdata) {
        const char* lastSlash = strrchr(_process, '\\');  // Use backslash for Windows
        data_home_folder = appdata;
        if (!lastSlash)
            data_home_folder = data_home_folder + "\\" + _process + "\\";
        else
            data_home_folder = data_home_folder + lastSlash + "\\";
    }
#endif
  return data_home_folder;
}

Workspace::Workspace(const char *_process) {
  std::string data_home_folder = this->getDataFolder(_process);

  if (data_home_folder.length() == 0)
    throw std::runtime_error("Cannot load data home");

  this->_config_file = data_home_folder + "config.cfg";
  this->_workspacelist_file = data_home_folder + "worspace.cfg";

  if (!fs::exists(data_home_folder)) {
    fs::create_directories(data_home_folder);
  }
  this->loadWorkspace();
}


Workspace::~Workspace() {
  this->_workspacelist_file.clear();
} 


void Workspace::loadWorkspace() {
    std::vector<std::string> lines = Workspace::getLines(this->_workspacelist_file.c_str());
    this->_projects.clear(); // Clear existing projects
    this->_projects.reserve(lines.size()); // Reserve space for efficiency

    for (const auto& line : lines) {
        if (line.empty()) continue;

        // Parse format: [project_name] - [path] - [version]
        std::vector<std::string> parts;
        std::stringstream ss(line);
        std::string part;


        while (std::getline(ss, part, '-')) {
            // Trim whitespace
            
            part.erase(0, part.find_first_not_of(" \t"));
            part.erase(part.find_last_not_of(" \t") + 1);
            parts.push_back(part);
        }

        if (parts.size() == 3) {
            this->_projects.emplace_back(parts[0], parts[1], parts[2]);
        }
    }
}


void Workspace::closeWorkspace() {
	this->_projects.clear();
}

std::vector<std::string> Workspace::getLines(const char* file)
{
	fs::path _path = fs::path(file);
    if(fs::exists(_path) && fs::is_regular_file(_path))
    {
        std::ifstream infile(file);
        if(infile.is_open())
        {
            std::vector<std::string> lines;
            std::string line;
            while(std::getline(infile, line))
                lines.push_back(line);
            infile.close();
            return lines;
        }
	}
    return std::vector<std::string>();
}
