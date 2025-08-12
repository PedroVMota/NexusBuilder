#include "Shader.h"





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

Shader::~Shader()
{
	if (p_fragmentCode)
		free(p_fragmentCode);
	if (p_vertexCode)
		free(p_vertexCode);

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
	if (!p_vertexCode) return; // Verifica se o código do vertex shader existe
	p_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (p_vertexShader == 0) return; // Falha ao criar shader
	glShaderSource(p_vertexShader, 1, &p_vertexCode, NULL);
	glCompileShader(p_vertexShader);

	// Verifica se a compilação foi bem-sucedida
	GLint success = 0;
	glGetShaderiv(p_vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(p_vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(p_vertexShader, logLength, NULL, infoLog);
			// Aqui você pode logar ou tratar o erro, por exemplo:
			// std::cerr << "Erro ao compilar vertex shader: " << infoLog << std::endl;
			delete[] infoLog;
		}
		glDeleteShader(p_vertexShader);
		p_vertexShader = 0;
	}
}


void Shader::compileFragment() {
	if (!p_fragmentCode) return; // Verifica se o código do fragment shader existe
	p_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (p_fragmentShader == 0) return; // Falha ao criar shader
	glShaderSource(p_fragmentShader, 1, &p_fragmentCode, NULL);
	glCompileShader(p_fragmentShader);
	// Verifica se a compilação foi bem-sucedida
	GLint success = 0;
	glGetShaderiv(p_fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(p_fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(p_fragmentShader, logLength, NULL, infoLog);
			delete[] infoLog;
		}
		glDeleteShader(p_fragmentShader);
		p_fragmentShader = 0;
	}
}

void Shader::compile() {
	this->p_shaderProgram = glCreateProgram();

	glAttachShader(p_shaderProgram, p_vertexShader);
	glAttachShader(p_shaderProgram, p_fragmentShader);
	glLinkProgram(p_shaderProgram);

	int success = -1;
	glGetProgramiv(p_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[1024];
		glGetProgramInfoLog(p_shaderProgram, 1024, NULL, infoLog);
		BOTAPICA_LOG_ERROR("Shader Error: " + std::string(infoLog));
	}
}