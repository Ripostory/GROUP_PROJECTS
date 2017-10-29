#ifndef CAMERALOC_H
#define CAMERALOC_H

#include "graphics_headers.h"
#include "event.h"
#include "object.h"
#include "imgui.h"

#define MAX_VELOCITY 1

class camera
{
  public:
    camera();
    ~camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void SetParent(Object*);
    void SetWorld(Object*);
    void Update(unsigned int);

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 lookAt;
    glm::vec3 position;
    event listener;
    Object *parent;
    Object *world;
    float fov;
    float speed;
    float velocity;
    int index;
    int w;
    int h;

    float xAngle;
    float yAngle;
};

#endif /* CAMERALOC_H */
