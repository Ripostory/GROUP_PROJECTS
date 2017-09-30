#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "Camera.h"
#include "shader.h"
#include "object.h"
#include "planet.h"
#include "moon.h"
#include "solarSystem.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    void RenderList(vector<Object*>);
    void TreeRender(Object*);
    bool InitShader(Shader*&, string, string);

  private:
    std::string ErrorString(GLenum error);

    camera *m_camera;
    Shader *m_shader;
    Shader *m_planetShader;
    Shader *m_gasGiantShader;
    Shader *m_screenShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint fbo;
    GLuint fbTex;
    GLuint screen;
    float vertices[24] = {
    	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
    	     1.0f,  1.0f, 1.0f, 1.0f, // Top-right
    	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right

    	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
    	    -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
    	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
        };

    SolarSystem *m_cube;
    Object *renderTarget;
};

#endif /* GRAPHICS_H */
