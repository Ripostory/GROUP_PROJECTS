#include "object.h"

Object::Object()
{  
	multiplier = 1.0f;
	size = 1.0f;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));

	//load default textures
	loadTexture("ERROR_TEXTURE.jpg");
	loadNormal("ERROR_NORMAL.png");
	loadModel("cube.obj");
}

Object::~Object()
{
  for (int i = 0; i < children.size(); i++)
  {
	  delete children[i];
  }

  children.clear();
}

void Object::init()
{
	loader::init();
}

void Object::end()
{
	loader::end();
}

void Object::Begin()
{
	//start children
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Begin();
	}
}

void Object::setVisual(string model, string albedo, string normal)
{
	loadModel(model);
	loadTexture(albedo);
	loadNormal(normal);
}

bool Object::loadModel(string filename)
{
	  loader modelLoader;
	  ModelInstance model = modelLoader.loadModel(filename);
	  if (model.VB != 0)
	  {
		  modelData = model;
		  return true;
	  }
	  else
		  return false;
}

bool Object::loadTexture(string filename)
{
	loader texLoader;
	textureData = texLoader.loadTexture(filename);
	if (textureData.texture == 0)
		return false;
	else
		return true;
}

void Object::loadTexture(string filename, int index)
{
	loader texLoader;
	texPointer.push_back(texLoader.loadTexture(filename, GL_TEXTURE0 + index).texture);
	texIndex.push_back(index);
}

void Object::loadCubeMap(string filename, GLuint &target)
{
	loader loadCube;
	target = loadCube.loadCubemap(filename).texture;
}

void Object::loadNormal(string filename)
{
	loader loadNorm;
	normal = loadNorm.loadTexture(filename, GL_TEXTURE1);
}

void Object::bindTex(GLuint &bind, GLenum unit)
{
	  bindTex(bind, unit, GL_TEXTURE_2D);
}

void Object::bindTex(GLuint &bind, GLenum unit, GLenum type)
{
	  glActiveTexture(unit);
	  glBindTexture(type, bind);
}

void Object::Update(unsigned int dt)
{
	  //render
	  model = mtranslate * mscale * mrotate;
	  //update keyboard
	  for (int i = 0; i < listener.getSize(); i++)
	  {
		  keyboard(listener.getEvent(i));
	  }

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
  glBindBuffer(GL_ARRAY_BUFFER, modelData.VB);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoord));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,tangent));

  bindTex(textureData.texture, GL_TEXTURE0);
  bindTex(normal.texture, GL_TEXTURE1);

  for (int i = 0; i < texPointer.size(); i++)
  {
	  bindTex(texPointer[i], GL_TEXTURE0 + texIndex[i]);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.IB);
  glDrawElements(GL_TRIANGLES, modelData.size, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
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
	mtranslate = glm::translate(translation);
	//reset model matrix
	model = mtranslate * mscale * mrotate;
}

void Object::rotate(float angle, glm::vec3 axis)
{
	mrotate = glm::rotate(angle, axis);
	//reset model matrix
	model = mtranslate * mscale * mrotate;
}

void Object::scale(float amount)
{
	size  = amount;
	mscale = glm::scale(glm::vec3(size));
	//reset model matrix
	model = mtranslate * mscale * mrotate;
}
