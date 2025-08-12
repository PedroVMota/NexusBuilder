#ifndef SHADER_H
#define SHADER_H

#include "main.h"

class Shader
{
private:
    char *p_fragmentCode = NULL;
    char *p_vertexCode = NULL;

    GLuint p_vertexShader = -1;
    GLuint p_fragmentShader = -1;
	GLuint p_shaderProgram = -1;

    void compileVertex();
    void compileFragment();
    void compile();

public:
    Shader() = delete;
    Shader(const Shader *_shader);
    Shader(const char *_fragCode, const char *_vertCode);
    ~Shader();

    Shader& operator=(const Shader& _shader);

    const char *getFragCode() const;
    const char *getVertCode() const;
    const GLuint& getShaderProgram() const;
};

// TODO: Add uniform feature.

#endif
