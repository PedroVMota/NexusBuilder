#ifndef SHADER_H
#define SHADER_H

#include "main.h"

class Shader
{
private:
    char *p_fragmentCode = NULL;
    char *p_vertexCode = NULL;

    GLuint p_vertexShader;
    GLuint p_fragmentShader;
    GLuint p_shaderProgram;

    void compileVertex();
    void compileFragment();
    void compile();

public:
    Shader() = delete;
    Shader(const Shader *_shader);
    Shader(const char *_fragCode, const char *_vertCode);
    Shader& operator=(const Shader& _shader);
    ~Shader();


    const char *getFragCode() const;
    const char *getVertCode() const;
    const GLuint& getShaderProgram() const;


    static const char* loadShader(char *_file);
};

// TODO: Add uniform feature.

#endif
