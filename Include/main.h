#pragma once

// Configuration macros
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#ifdef DEBUG
	#define BOTAPICA_DEBUG
	#define BOTAPICA_ENABLE_ASSERTS
#else
	#define BOTAPICA_RELEASE
#endif

// Standard includes
#include <iostream>
#include <sstream>
#include <string>

// Add LogLevel enum
enum class LogLevel {
	INFO,
	WARNING,
	ERROR,
	DEBUG_LEVEL
};

// Utility macros
#define BOTAPICA_EXPAND_MACRO(x) x
#define BOTAPICA_STRINGIFY_MACRO(x) #x

// Assertion macros
#ifdef BOTAPICA_ENABLE_ASSERTS
	#include <cassert>
	#define BOTAPICA_ASSERT(x, ...) { if(!(x)) { std::cerr << "Assertion Failed: " << __VA_ARGS__ << std::endl; assert(false); } }
	#define BOTAPICA_CORE_ASSERT(x, ...) { if(!(x)) { std::cerr << "Core Assertion Failed: " << __VA_ARGS__ << std::endl; assert(false); } }
#else
	#define BOTAPICA_ASSERT(x, ...)
	#define BOTAPICA_CORE_ASSERT(x, ...)
#endif

// Global logging function pointer
extern void (*g_consoleLogFunc)(const char* level, const char* message);

// Helper function for console logging
inline void LogToConsole(const char* level, const std::string& message) {
	// First try to log to console if available
	if (g_consoleLogFunc) {
		g_consoleLogFunc(level, message.c_str());
	}
	// Also log to stdout/stderr as fallback
	if (std::string(level) == "ERROR" || std::string(level) == "FATAL") {
		std::cerr << "[" << level << "] " << message << std::endl;
	} else {
		std::cout << "[" << level << "] " << message << std::endl;
	}
}

// Logging macros
#define BOTAPICA_LOG_TRACE(...)    do { \
	std::stringstream ss; ss << __VA_ARGS__; \
	LogToConsole("TRACE", ss.str()); \
} while(0)

#define BOTAPICA_LOG_INFO(...)     do { \
	std::stringstream ss; ss << __VA_ARGS__; \
	LogToConsole("INFO", ss.str()); \
} while(0)

#define BOTAPICA_LOG_WARN(...)     do { \
	std::stringstream ss; ss << __VA_ARGS__; \
	LogToConsole("WARN", ss.str()); \
} while(0)

#define BOTAPICA_LOG_ERROR(...)    do { \
	std::stringstream ss; ss << __VA_ARGS__; \
	LogToConsole("ERROR", ss.str()); \
} while(0)

#define BOTAPICA_LOG_FATAL(...)    do { \
	std::stringstream ss; ss << __VA_ARGS__; \
	LogToConsole("FATAL", ss.str()); \
} while(0)

// Math constants
#define BOTAPICA_PI 3.14159265358979323846f
#define BOTAPICA_TWO_PI (2.0f * BOTAPICA_PI)
#define BOTAPICA_HALF_PI (BOTAPICA_PI / 2.0f)
#define BOTAPICA_DEG_TO_RAD (BOTAPICA_PI / 180.0f)
#define BOTAPICA_RAD_TO_DEG (180.0f / BOTAPICA_PI)

//Window Macros
#define WIDHT 1920
#define HEIGHT 1080

// OpenGL and GLEW (must be included before GLFW)
#include <GL/glew.h>

// GLFW for window management
#include <GLFW/glfw3.h>

// ImGui for immediate-mode GUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Editors.h"

// GLM for mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>

// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>



// Type aliases for convenience
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using String = std::string;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

// GLM type aliases
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;
using Quat = glm::quat;
