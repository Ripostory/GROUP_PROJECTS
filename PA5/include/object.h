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
    Object(string, float, float, float);
    virtual ~Object();
    virtual void Update(unsigned int dt);
    void loadNewModel(string filename);
    void Render();

    glm::mat4 GetModel();

  protected:
    glm::mat4 model;

    float angle;
    float orbit;
    float xPos;
    float yPos;

    float distance;
    float orbitSpeed;
    float rotationSpeed;
    event listener;
    virtual void keyboard(eventType);

  private:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;


};

#endif /* OBJECT_H */
