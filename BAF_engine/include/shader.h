#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <stdio.h>
#include "loader.h"
#include "graphics_headers.h"
#include "imgui.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, string filename);
    bool Finalize();
    bool InitShader(string vertex, string fragment);
    GLint GetUniformLocation(const char* pUniformName);
    GLint getShader();

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
    string name;
};

#endif  /* SHADER_H */
