#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "Camera.h"
#include "shader.h"
#include "object.h"
#include "world.h"
#include "billboard.h"
#include "gui.h"
#include "framebuffer.h"
#include "ocean.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, SDL_Window *window);
    void Update(unsigned int dt);
    void Render();

  private:
    std::string ErrorString(GLenum error);
    void RenderList(vector<Object*>);
    void TreeRender(Object*);
    bool InitShader(Shader*&, string, string);
    void renderDeferred(Shader *shader, Light *light);
    void renderSkybox(Shader *shader);
    void beginFBODraw(int width, int height);
    void updateFPS(unsigned int dt);
    void passMatrices(glm::mat4 modelMatrix);
    void drawQuad();

    GUI ui;

    camera *m_camera;
    Shader *m_shader;
    Shader *m_billboard;
    Shader *m_skyboxShader;
    Shader *m_deferredShader;
    Shader *m_deferredOcean;
    Shader *m_ambientShader;
    Shader *m_pointShader;
    Shader *m_directionShader;
    Shader *m_screenShader;
    Shader *m_HDRShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLuint screen;

    FrameBuffer m_deferredFBO;
    FrameBuffer m_lightingFBO;

    World *world;
    Object *renderTarget;
    Ocean *ocean;

    float resScale;

    int width;
    int height;
    int frameCount;
    float fps;
    unsigned int delay;

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
