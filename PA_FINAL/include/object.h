#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "obj.h"
#include "event.h"
#include "Animator.h"

#define SPEED_STEP 		0.02f

class Object
{
  public:
    Object();
    virtual ~Object();

    static void init();
    static void end();

    bool loadModel(string filename);
    bool loadTexture(string filename);
    void loadTexture(string filename, int index);
    void loadCubeMap(string filename, GLuint &target);
    void loadNormal(string filename);

    virtual void Update(unsigned int dt);
    virtual void Begin();
    virtual void Render();
    void setVisual(string, string, string);
    void setMultiplier(float);
    void addChild(Object*);
    virtual void translate(glm::vec3);
    virtual void rotate(float angle, glm::vec3 axis);
    virtual void translateBy(glm::vec3);
    virtual void rotateBy(float angle, glm::vec3 axis);
    virtual void scale(float amount);

    float getSize();
    glm::mat4 GetModel();
    std::vector<Object*> getChildren();

    Animator animator;

  protected:
    glm::mat4 model;
    glm::mat4 mtranslate;
    glm::mat4 mscale;
    glm::mat4 mrotate;
    std::vector<Object*> children;
    std::vector<unsigned int> texIndex;
    std::vector<GLuint> texPointer;
    event listener;
    ModelInstance modelData;
    float multiplier;
    float size;
    void bindTex(GLuint&, GLenum);
    void bindTex(GLuint&, GLenum, GLenum);

    virtual void keyboard(eventType);

  private:
    TexInstance textureData;
    TexInstance normal;
};

#endif /* OBJECT_H */