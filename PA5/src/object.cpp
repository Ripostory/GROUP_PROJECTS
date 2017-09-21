#include "object.h"

Object::Object()
{  
  //updated file loading
  loadNewModel("assets/board.obj");

  angle = 0.0f;
  orbit = 0.0f;

  rotationSpeed = 0.3f;
  orbitSpeed = 0.0f;
  distance = 0.0f;
}

Object::Object(string filename, float rotSpeed, float orbSpeed, float dist)
{
	  loadNewModel(filename);

	  angle = 0.0f;
	  orbit = 0.0f;

	  rotationSpeed = rotSpeed;
	  orbitSpeed = orbSpeed;
	  distance = dist;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::loadNewModel(string filename)
{
	  loader fileLoader;
	  obj object;
	  if (fileLoader.loadObject(filename, object))
	  {
		  Vertices = object.getVerts();
		  Indices = object.getIndices();
	  }

	  glGenBuffers(1, &VB);
	  glBindBuffer(GL_ARRAY_BUFFER, VB);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	  glGenBuffers(1, &IB);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void Object::Update(unsigned int dt)
{
  //listen for event
  for (int i = 0 ; i < listener.getSize(); i++) {
	  keyboard(listener.getEvent(i));
  }
  //calculate orbit and convert to position matrix
  orbit -= orbitSpeed * dt * M_PI/1000;
  xPos = glm::sin(orbit);
  yPos = glm::cos(orbit);
  model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos*distance, 0.0, yPos*distance));

  //original rotate code modified to take initial translated matrix
  angle += rotationSpeed * dt * M_PI/1000;
  model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));
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

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

