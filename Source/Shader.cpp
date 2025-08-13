#include "Shader.h"
#include "Material.h"
#include <fstream>
#include <cstring>

Shader::Shader(const Shader *_shader) { *this = *_shader; }

const char* Shader::getFragCode() const { return p_fragmentCode; }

const char* Shader::getVertCode() const { return p_vertexCode; }

const GLuint& Shader::getShaderProgram() const { return p_shaderProgram;  }


Shader::Shader(const char *_fragCode, const char *_vertCode)
{
	p_fragmentCode = strdup(_fragCode);
	p_vertexCode = strdup(_vertCode);
	compileVertex();
	compileFragment();
	compile();
}

Shader & Shader::operator=(const Shader& _shader) {
	if (this != &_shader) {
		if (p_fragmentCode) free(p_fragmentCode);
		if (p_vertexCode) free(p_vertexCode);
		p_fragmentCode = strdup(_shader.p_fragmentCode);
		p_vertexCode = strdup(_shader.p_vertexCode);
		p_vertexShader = _shader.p_vertexShader;
		p_fragmentShader = _shader.p_fragmentShader;
		p_shaderProgram = _shader.p_shaderProgram;
	}
	return *this;
}

void Shader::compileVertex() {
    if (!p_vertexCode) {
        BOTAPICA_LOG_ERROR("Vertex shader code is null");
        exit(EXIT_FAILURE);
    }

    p_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (p_vertexShader == 0) {
        BOTAPICA_LOG_ERROR("Failed to create vertex shader");
        exit(EXIT_FAILURE);
    }

    glShaderSource(p_vertexShader, 1, &p_vertexCode, NULL);
    glCompileShader(p_vertexShader);

    GLint success = 0;
    glGetShaderiv(p_vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(p_vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char* infoLog = new char[logLength];
            glGetShaderInfoLog(p_vertexShader, logLength, NULL, infoLog);
            BOTAPICA_LOG_ERROR("Vertex shader compilation failed: " + std::string(infoLog));
            delete[] infoLog;
        }
        glDeleteShader(p_vertexShader);
        exit(EXIT_FAILURE);
    }
}

void Shader::compileFragment() {
    if (!p_fragmentCode) {
        BOTAPICA_LOG_ERROR("Fragment shader code is null");
        exit(EXIT_FAILURE);
    }

    p_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (p_fragmentShader == 0) {
        BOTAPICA_LOG_ERROR("Failed to create fragment shader");
        exit(EXIT_FAILURE);
    }

    glShaderSource(p_fragmentShader, 1, &p_fragmentCode, NULL);
    glCompileShader(p_fragmentShader);

    GLint success = 0;
    glGetShaderiv(p_fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(p_fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            char* infoLog = new char[logLength];
            glGetShaderInfoLog(p_fragmentShader, logLength, NULL, infoLog);
            BOTAPICA_LOG_ERROR("Fragment shader compilation failed: " + std::string(infoLog));
            delete[] infoLog;
        }
        glDeleteShader(p_fragmentShader);
        exit(EXIT_FAILURE);
    }
}

void Shader::compile() {
    if (p_vertexShader == 0 || p_fragmentShader == 0) {
        BOTAPICA_LOG_ERROR("Vertex or fragment shader not compiled");
        exit(EXIT_FAILURE);
    }

    this->p_shaderProgram = glCreateProgram();
    if (p_shaderProgram == 0) {
        BOTAPICA_LOG_ERROR("Failed to create shader program");
        exit(EXIT_FAILURE);
    }

    glAttachShader(p_shaderProgram, p_vertexShader);
    glAttachShader(p_shaderProgram, p_fragmentShader);
    glLinkProgram(p_shaderProgram);

    int success = -1;
    glGetProgramiv(p_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(p_shaderProgram, 1024, NULL, infoLog);
        BOTAPICA_LOG_ERROR("Shader linking failed: " + std::string(infoLog));
        exit(EXIT_FAILURE);
    }
}


Shader::~Shader() {
	if (p_fragmentCode) free(p_fragmentCode);
	if (p_vertexCode) free(p_vertexCode);
	if (p_vertexShader != -1) glDeleteShader(p_vertexShader);
	if (p_fragmentShader != -1) glDeleteShader(p_fragmentShader);
	if (p_shaderProgram != -1) glDeleteProgram(p_shaderProgram);
}


// Implement MaterialShaderInterface methods
void Shader::BindMaterialUniforms(const Material& material) {
    glUseProgram(p_shaderProgram);
    
    // Get uniform locations and set material properties
    GLint ambientLoc = glGetUniformLocation(p_shaderProgram, "materialAmbient");
    GLint diffuseLoc = glGetUniformLocation(p_shaderProgram, "materialDiffuse");
    GLint specularLoc = glGetUniformLocation(p_shaderProgram, "materialSpecular");
    GLint shininessLoc = glGetUniformLocation(p_shaderProgram, "materialShininess");
    
    if (ambientLoc != -1) {
        const Vec3& ambient = material.GetAmbient();
        glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);
    }
    
    if (diffuseLoc != -1) {
        const Vec3& diffuse = material.GetDiffuse();
        glUniform3f(diffuseLoc, diffuse.x, diffuse.y, diffuse.z);
    }
    
    if (specularLoc != -1) {
        const Vec3& specular = material.GetSpecular();
        glUniform3f(specularLoc, specular.x, specular.y, specular.z);
    }
    
    if (shininessLoc != -1) {
        glUniform1f(shininessLoc, material.GetShininess());
    }
}

bool Shader::IsCompatibleWith(const Material& material) {
    // Check if shader has required uniforms for this material
    glUseProgram(p_shaderProgram);
    
    GLint ambientLoc = glGetUniformLocation(p_shaderProgram, "materialAmbient");
    GLint diffuseLoc = glGetUniformLocation(p_shaderProgram, "materialDiffuse");
    GLint specularLoc = glGetUniformLocation(p_shaderProgram, "materialSpecular");
    GLint shininessLoc = glGetUniformLocation(p_shaderProgram, "materialShininess");
    
    // Basic compatibility: shader should have at least diffuse uniform
    return diffuseLoc != -1;
}

void Shader::Use() {
    glUseProgram(p_shaderProgram);
}

const char* Shader::loadShader(char* _file) {
    std::ifstream ifs(_file);
    if (!ifs.is_open()) {
        BOTAPICA_LOG_ERROR("Error opening the file");
        return NULL;
    }
    std::string content;
    std::string line;
    while (std::getline(ifs, line)) {
        content += line + "\n";  // Your original loop only kept the last line
    }
    // Allocate memory that persists after function returns
    char* result = new char[content.length() + 1];
    std::strcpy(result, content.c_str());
    return result;
}