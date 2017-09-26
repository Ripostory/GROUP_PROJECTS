#include "object.h"

Object::Object()
{  
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

	  //model loading
	  glGenBuffers(1, &VB);
	  glBindBuffer(GL_ARRAY_BUFFER, VB);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	  glGenBuffers(1, &IB);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


}

void Object::loadNewTexture(string filename)
{
	  loader fileLoader;

	  Texture texture(0,0,NULL);
	  if (fileLoader.loadTexture(filename, texture))
	  {
		  //texture loaded
		  //TODO: probably do something more
	  }

	  //texture loading
	  glGenTextures(1, &tex);
	  glBindTexture(GL_TEXTURE_2D, tex);

	  cout << texture.data << endl;
	  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.column, texture.row, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	  //texture choosing
	  //TODO implement
	  //glActiveTexture(GL_TEXTURE1);
	  //glBindTexture(GL_TEXTURE_2D, tex);
}

void Object::Update(unsigned int dt)
{
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
  //glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  //glDisableVertexAttribArray(2);
}

