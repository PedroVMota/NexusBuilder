# Botapica

A modern 3D game engine built with OpenGL and C++17, featuring an immediate-mode GUI for development tools and real-time editing.

## 🎯 Project Purpose

Botapica is designed to be a lightweight, cross-platform 3D game engine that provides:
- Real-time 3D rendering with OpenGL
- Integrated development tools using ImGui
- Cross-platform window management
- Modern C++ architecture
- Easy-to-use math library for 3D calculations

## 🛠️ Libraries & Dependencies

### Core Libraries
- **[GLFW](https://github.com/glfw/glfw)** - Cross-platform window creation and input handling
- **[GLEW](https://github.com/nigels-com/glew)** - OpenGL Extension Wrangler for function loading
- **[ImGui](https://github.com/ocornut/imgui)** - Immediate-mode GUI for development tools and debugging
- **[GLM](https://github.com/g-truc/glm)** - OpenGL Mathematics library for vectors, matrices, and transformations

### Build System
- **Premake5** - Cross-platform build configuration
- **Make** - Build automation (Linux)

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler (GCC, Clang, MSVC)
- Premake5
- OpenGL development libraries
- X11 development libraries (Linux only)

### Building on Linux

1. **Clone the repository with submodules:**
   ```bash
   git clone --recursive https://github.com/PedroVMota/Botapica.git
   cd Botapica
   ```

2. **Install dependencies (Ubuntu/Debian):**
   ```bash
   sudo apt install build-essential libx11-dev libxcursor-dev libxinerama-dev libxrandr-dev libxi-dev libgl1-mesa-dev
   ```

3. **Generate build files:**
   ```bash
   premake5 gmake2
   ```

4. **Build the project:**
   ```bash
   make
   ```

5. **Run the engine:**
   ```bash
   ./bin/Debug-linux-x86_64/Botapica
   ```

### For Team Development

If you're cloning an existing repository, initialize the submodules:
```bash
git clone https://github.com/PedroVMota/Botapica.git
cd Botapica
git submodule update --init --recursive
```

## 📁 Project Structure

```
Botapica/
├── Source/           # Main source code
├── Include/          # Header files
├── lib/              # Third-party libraries (as git submodules)
│   ├── glfw/         # Window management
│   ├── glew/         # OpenGL function loading
│   ├── imgui/        # Immediate-mode GUI
│   └── glm/          # Math library
├── bin/              # Compiled binaries
├── bin-int/          # Intermediate build files
├── premake5.lua      # Build configuration
└── README.md
```

## 🎮 Features (Planned)

- [ ] Render Pipeline
- [ ] 3D Viewport
- [ ] File manager
- [ ] Menus & Settings
- [ ] 3D Model Loading
- [ ] Shader Management System
- [ ] Scene Graph
- [ ] Camera System
- [ ] Lighting System
- [ ] Material System
- [ ] Physics Integration
- [ ] Asset Management
- [ ] Level Editor
- [ ] Scripting Support

## 🔗 Links

- [GLFW Documentation](https://www.glfw.org/docs/latest/)
- [OpenGL Documentation](https://docs.gl/)
- [ImGui Documentation](https://github.com/ocornut/imgui/wiki)
- [GLM Documentation](https://glm.g-truc.net/0.9.9/index.html)

