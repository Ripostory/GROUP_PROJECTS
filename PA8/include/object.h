#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "obj.h"
#include "event.h"
#include <bullet/btBulletDynamicsCommon.h>


#define SPEED_STEP 		0.02f

class Object
{
  public:
    Object();
    virtual ~Object();

    static void init();

    bool loadNewModel(string filename);
    bool loadNewTexture(string filename);

    virtual void Update(unsigned int dt);
    virtual void Render();
    void loadNewTexture(string filename, int index);
    void loadNewNormal(string filename);
    void setVisual(string, string, string);
    void setMultiplier(float);
    void setTex(Texture);
    void addChild(Object*);
    void initPhyiscs();
    void translate(glm::vec3);

    float getSize();
    glm::mat4 GetModel();
    std::vector<Object*> getChildren();
    GLuint bindTex(GLuint&, GLenum);

  protected:
    glm::mat4 model;
    std::vector<Object*> children;
    event listener;
    virtual void keyboard(eventType);
    float multiplier;
    float size;

  private:
    static string rootDir;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<unsigned int> texIndex;
    std::vector<GLuint> texPointer;
    std::vector<Texture> texture;
    GLuint VB;
    GLuint IB;
    GLuint tex;
    GLuint normal;
    Texture albedo;
    Texture normalMap;
    btRigidBody *physics;
    btTransform transform;
    float mass;
};

#endif /* OBJECT_H */
