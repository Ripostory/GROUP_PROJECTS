#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "obj.h"
#include "event.h"

#define SPEED_STEP 0.2f

class Object
{
  public:
    Object();
    virtual ~Object();
    virtual void Update(unsigned int dt);
    void loadNewModel(string filename);
    void loadNewTexture(string filename);
    void loadNewNormal(string filename);
    void setMultiplier(float);
    GLuint bindTex(GLuint&, GLenum);
    void setTex(Texture);
    virtual void Render();

    glm::mat4 GetModel();

  protected:
    glm::mat4 model;
    event listener;
    virtual void keyboard(eventType);
    float multiplier;

  private:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint tex;
    GLuint normal;
    Texture albedo;
    Texture normalMap;


};

#endif /* OBJECT_H */
