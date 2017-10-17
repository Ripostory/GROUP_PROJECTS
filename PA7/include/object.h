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
    bool loadNewModel(string filename);
    bool loadNewTexture(string filename);
    void loadNewTexture(string filename, int index);
    void loadNewNormal(string filename);
    void setVisual(string, string, string);
    void setMultiplier(float);
    GLuint bindTex(GLuint&, GLenum);
    void setTex(Texture);
    void setGasGiant(bool);
    void setEarth(bool earth);
    void setRing(bool ring);
		void setSkyBox (bool value);
    virtual void Render();
    bool isaPlanet();
    bool isaGasGiant();
    bool isEarth();
    bool isRing();
		bool isaSkyBox ();
    glm::vec3 getHorizon();
    glm::vec3 getAtmosphere();
    void setHorizon(glm::vec3);
    void setAtmosphere(glm::vec3);
    static void init();

    glm::mat4 GetModel();
    float getSize();

    std::vector<Object*> getChildren();
    void addChild(Object*);

		friend std::ostream& operator<< (std::ostream& os, const Object& object);
		friend std::ostream& operator>> (std::istream& os, Object& object);

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
		bool isSkyBox;
    float size;
    glm::vec3 horizonColor;
    glm::vec3 atmosphereColor;

		string model_path;
		std::vector<string> texture_paths;

		void AppendSaveFile (ofstream& save);

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

};

#endif /* OBJECT_H */
