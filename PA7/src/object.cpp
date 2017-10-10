#include "object.h"

string Object::rootDir = "assets/";

Object::Object()
{  
	albedo = Texture(0,0,NULL);
	normalMap = Texture(0,0,NULL);
	isPlanet = false;
	isGasGiant = false;
	earth = false;
	isaRing = false;
	horizonColor = glm::vec3(0.0f);
	atmosphereColor = glm::vec3(0.0f);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::init()
{
	//initialize root directory
	Object test;
	if (!test.loadNewModel("models/cube.obj"))
	{
		rootDir = "../assets/";
		cout << "ROOT CHANGED" << endl;
	}
}

void Object::setVisual(string model, string albedo, string normal)
{
	loadNewModel(model);
	loadNewTexture(albedo);
	loadNewNormal(normal);
}

bool Object::loadNewModel(string filename)
{

		model_path = filename;


	  bool success = false;
	  loader fileLoader;
	  obj object;
	  if (fileLoader.loadObject(rootDir + filename, object))
	  {
		  Vertices = object.getVerts();
		  Indices = object.getIndices();
		  success = true;
	  }

	  //model loading
	  glGenBuffers(1, &VB);
	  glBindBuffer(GL_ARRAY_BUFFER, VB);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (Vertices.size()+1), &Vertices[0], GL_STATIC_DRAW);

	  glGenBuffers(1, &IB);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	  return success;
}

bool Object::loadNewTexture(string filename)
{

		texture_paths.push_back (filename);

	  loader fileLoader;

	  Texture texture(0,0,NULL);
	  if (fileLoader.loadTexture(rootDir + filename, texture))
	  {
		  //texture loaded
		  albedo = texture;
		  glGenTextures(1, &tex);
		  bindTex(tex, GL_TEXTURE0);
		  setTex(albedo);
		  return true;
	  }
	  return false;
}

void Object::loadNewTexture(string filename, int index)
{
	  loader fileLoader;

		texture_paths.push_back (filename);

	  Texture tex(0,0,NULL);
	  if (fileLoader.loadTexture(rootDir + filename, tex))
	  {
		  GLuint GLpointer;
		  //texture loaded
		  glGenTextures(1, &GLpointer);
		  bindTex(GLpointer, GL_TEXTURE0 + index);
		  texIndex.push_back(index);
		  texPointer.push_back(GLpointer);
		  texture.push_back(tex);
		  setTex(texture[texture.size()-1]);
	  }
}

void Object::loadNewNormal(string filename)
{
	  loader fileLoader;

		texture_paths.push_back (filename);

	  Texture texture(0,0,NULL);
	  if (fileLoader.loadTexture(rootDir + filename, texture))
	  {
		  //texture loaded
		  normalMap = texture;
		  glGenTextures(1, &normal);
		  bindTex(normal, GL_TEXTURE1);
		  setTex(normalMap);
	  }
}

GLuint Object::bindTex(GLuint &bind, GLenum unit)
{
	  glActiveTexture(unit);
	  glBindTexture(GL_TEXTURE_2D, bind);
}

void Object::setTex(Texture texture)
{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.column, texture.row, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Object::Update(unsigned int dt)
{
}

void Object::setMultiplier(float mult)
{
	multiplier = mult;
}

void Object::keyboard(eventType event)
{
	//does nothing, is virtual for inherited objects
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

  bindTex(tex, GL_TEXTURE0);
  bindTex(normal, GL_TEXTURE1);

  for (int i = 0; i < texPointer.size(); i++)
  {
	  bindTex(texPointer[i], GL_TEXTURE0 + texIndex[i]);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

std::vector<Object*> Object::getChildren()
{
	return children;
}
void Object::addChild(Object *child)
{
	children.push_back(child);
}

bool Object::isaPlanet()
{
	return isPlanet;
}

bool Object::isaGasGiant()
{
	return isGasGiant;
}

void Object::setGasGiant(bool isGas)
{
	isGasGiant = isGas;
}

float Object::getSize()
{
	return size;
}

bool Object::isEarth()
{
	return earth;
}

void Object::setEarth(bool isEarth)
{
	earth = isEarth;
}

bool Object::isRing()
{
	return isaRing;
}

void Object::setRing(bool ring)
{
	isaRing= ring;
}

glm::vec3 Object::getHorizon()
{
	return horizonColor;
}

glm::vec3 Object::getAtmosphere()
{
	return atmosphereColor;
}

void Object::setHorizon(glm::vec3 horizon)
{
	horizonColor = horizon;
}

void Object::setAtmosphere(glm::vec3 atmosphere)
{
	atmosphereColor = atmosphere;
}


