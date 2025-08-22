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

std::string Workspace::getDataFolder(const char *_process) const {

  const char *lastSlash = strrchr(_process, '/');
  std::string data_home_folder = getenv("XDG_DATA_HOME");

  if (!lastSlash)
    data_home_folder = data_home_folder + "/" + _process + "/";
  else
    data_home_folder = data_home_folder + lastSlash + "/";

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

  FileEnviroment config = this->load(this->_config_file);
  FileEnviroment workspace = this->load(this->_workspacelist_file);

  std::cout << "Data: " << data_home_folder << "\n";
  std::cout << "Config: " << _config_file << "\n";
  std::cout << "Workspace: " << _workspacelist_file << "\n";

  if (!this->_opened_files.count(_config_file)) {
    this->_opened_files[_config_file] = config;
  }

  if (!this->_opened_files.count(_workspacelist_file)) {
    this->_opened_files[_workspacelist_file] = workspace;
  }

  std::map<std::string, FileEnviroment>::iterator it;
  for (it = this->_opened_files.begin(); it != this->_opened_files.end(); it++) {
    BOTAPICA_LOG_INFO("Detected: " << it->second.filepath);
  }
}
Workspace::~Workspace() {
  this->_workspacelist_file.clear();
  this->_opened_files.clear();
} 

FileEnviroment Workspace::load(const std::string &fullpathfile) {
  FileEnviroment env;
  env.file = std::make_shared<std::ofstream>(fullpathfile);
  env.filepath = fullpathfile;
  if (!env.file->is_open()) {
    throw std::runtime_error("Couldn't open file: " + fullpathfile + "\n");
  }
  std::ifstream istrm(fullpathfile, std::ios::binary);
  double d;
  istrm.read(reinterpret_cast<char *>(&d), sizeof d);
  int n;
  std::string s;
  istrm >> n >> s;
  env.content = std::make_shared<std::string>(s);
  return env;
}

void Workspace::loadWorkspace() {};
void Workspace::closeWorkspace() {};
