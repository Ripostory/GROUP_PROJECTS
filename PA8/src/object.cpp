#include "object.h"

string Object::rootDir = "assets/";

Object::Object()
{  
	physics = NULL;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
	initPhyiscs();
	loadNewModel("models/planet.obj");
	loadNewTexture("textures/a_earth.jpg");
	loadNewNormal("textures/n_earth.jpg");
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
  listener.getWorld()->removeCollisionObject(physics);
  delete physics;
  glDeleteBuffers(1, &IB);
  glDeleteBuffers(1, &VB);

  for (int i = 0; i < children.size(); i++)
  {
	  delete children[i];
  }

  children.clear();
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
	  bool success = false;
	  loader fileLoader;
	  obj object;
	  if (fileLoader.loadObject(rootDir + filename, object))
	  {
		  Vertices.clear();
		  Indices.clear();
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
	  //update physics object
	  if (physics != NULL)
		  physics->getMotionState()->getWorldTransform(transform);
	  model = glm::translate(glm::mat4(1.0f), glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()));

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
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
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

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

float Object::getSize()
{
	return size;
}

void Object::translate(glm::vec3 translation)
{
	model = glm::translate(model, translation);
}

void Object::initPhyiscs()
{
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState =
	                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
	                groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_restitution = 0.1f;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	listener.getWorld()->addRigidBody(groundRigidBody);


	transform.setIdentity();
	transform.setOrigin(btVector3(0, 25, 0));
	mass = 0.2f;
	btVector3 inertia(0,0,0);
	btCollisionShape* shape = new btSphereShape(1);
	shape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* objMotionState =
	                new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo
	                objCI(mass, objMotionState, shape, inertia);
	objCI.m_restitution = 0.1f;

	physics = new btRigidBody(objCI);
	listener.getWorld()->addRigidBody(physics);

}
