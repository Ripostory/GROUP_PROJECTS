#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "Camera.h"
#include "shader.h"
#include "object.h"

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
    void addRenderTarget(Shader *shader, GLuint texTarget);
    void generateFrameBuffer(GLuint &fbo, GLuint &fbTarget, int width, int height);

  private:
    std::string ErrorString(GLenum error);

    camera *m_camera;
    Shader *m_shader;
    Shader *m_screenShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint screen;

    GLuint fbo;
    GLuint fbTex;

    Object *world;
    Object *renderTarget;

    int width;
    int height;

    float vertices[24] = {
    	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
    	     1.0f,  1.0f, 1.0f, 1.0f, // Top-right
    	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right

    	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
    	    -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
    	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
        };
};

#endif /* GRAPHICS_H */
