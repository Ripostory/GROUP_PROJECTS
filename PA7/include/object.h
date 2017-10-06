#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "obj.h"
#include "event.h"

#define ROTATION_MULT	0.002f
#define ORBIT_MULT		0.2f
#define DISTANCE_MULT	10.0f
#define SIZE_MULT		0.2f
#define SPEED_STEP 		0.02f

class Object
{
  public:
    Object();
    virtual ~Object();
    virtual void Update(unsigned int dt);
    void loadNewModel(string filename);
    void loadNewTexture(string filename);
    void loadNewTexture(string filename, int index);
    void loadNewNormal(string filename);
    void setVisual(string, string, string);
    void setMultiplier(float);
    GLuint bindTex(GLuint&, GLenum);
    void setTex(Texture);
    void setGasGiant(bool);
    void setEarth(bool earth);
    void setRing(bool ring);
    virtual void Render();
    bool isaPlanet();
    bool isaGasGiant();
    bool isEarth();
    bool isRing();
    glm::vec3 getHorizon();
    glm::vec3 getAtmosphere();
    void setHorizon(glm::vec3);
    void setAtmosphere(glm::vec3);

    glm::mat4 GetModel();
    float getSize();

    std::vector<Object*> getChildren();
    void addChild(Object*);
  protected:
    glm::mat4 model;
    event listener;
    virtual void keyboard(eventType);
    float multiplier;
    std::vector<Object*> children;
    bool isPlanet;
    bool isGasGiant;
    bool earth;
    bool isaRing;
    string rootDir;
    float size;
    glm::vec3 horizonColor;
    glm::vec3 atmosphereColor;

  private:
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
};

#endif /* OBJECT_H */
