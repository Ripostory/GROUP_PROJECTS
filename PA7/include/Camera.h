#ifndef CAMERALOC_H
#define CAMERALOC_H

#include "graphics_headers.h"
#include "event.h"

class camera
{
  public:
    camera();
    ~camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void SetParent(glm::mat4*);
    void Update(unsigned int);

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 *parent;
    glm::vec4 lookAt;
    glm::vec3 position;
    event listener;

    float orbit;
    float xPos;
    float yPos;
    float distance;
    float height;
};

#endif /* CAMERALOC_H */
