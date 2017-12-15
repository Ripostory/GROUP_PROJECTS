#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "obj.h"
#include "event.h"
#include "Animator.h"
#include "billboard.h"

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
    virtual void rotateTo(glm::vec3 lookat, glm::vec3 up);
    virtual void scale(float amount);

    void lerpX(float, float, interpolation);
    void lerpY(float, float, interpolation);
    void lerpZ(float, float, interpolation);

    void lerpXRot(float, float, interpolation);
    void lerpYRot(float, float, interpolation);
    void lerpZRot(float, float, interpolation);

    void lerpTo(glm::vec3, float);
    void lerpBy(glm::vec3, float);

    float getSize();
    glm::mat4 GetModel();
    std::vector<Object*> getChildren();

    Animator animator;

    void addUI(Billboard*);
    void renderUI();
    void setParent(Object*);
    std::vector<Billboard*> getUI();

  protected:
    glm::mat4 model;
    glm::mat4 mtranslate;
    glm::mat4 mscale;
    glm::mat4 mrotate;
    std::vector<Object*> children;
    std::vector<Billboard*> ui;
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
    void updateMatrices();

    Object *parent;
    TexInstance textureData;
    TexInstance normal;
};

#endif /* OBJECT_H */
