/*
 * loader.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#include <loader.h>

vector<ModelInstance> loader::modelBank;
vector<TexInstance> loader::textureBank;
string loader::rootDir = "assets/";

loader::loader()
{
	isEOF = false;
	isAnnoying = false;
}

loader::~loader()
{
	isEOF = false;
	isAnnoying = false;
}

bool loader::loadObject(string filename, obj &inputObj)
{
	aiMesh *mesh;
	aiVector3D vert;
	aiVector3D texture;
	aiFace face;
	glm::vec3 tempVec;
	glm::vec3 tempColor;
	glm::vec2 tempTex;
	glm::vec3 tempTan;
	glm::vec3 bitangent;
	int indice;
	string finalFilename = rootDir + "models/" + filename;

	//load object from file
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(finalFilename, \
			aiProcess_CalcTangentSpace      |  \
			aiProcess_GenNormals            |  \
			aiProcess_JoinIdenticalVertices |  \
			aiProcess_Triangulate           |  \
			aiProcess_SortByPType			|  \
			aiProcess_FlipUVs				|  \
			0
			);

	//if fail, throw error
	if (scene == NULL)
	{
		//read failed, return empty object
		cout << "FILE " << finalFilename << " UNABLE TO BE READ" << endl;
		return false;
	}

	//otherwise, pack object data into the OBJ ADT and send to inputObj
	//get mesh (should be at index 0 since it's the only mesh)
	mesh = scene->mMeshes[0];

	obj final;
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//get vertices
		vert = mesh->mVertices[i];
		tempVec.x = vert.x;
		tempVec.y = vert.y;
		tempVec.z = vert.z;

		//get color (get them from normals)
		vert = mesh->mNormals[i];
		tempColor.r = vert.x;
		tempColor.g = vert.y;
		tempColor.b = vert.z;

		//get texture coordinates and tangents
		if (mesh->HasTextureCoords(0))
		{
			texture = mesh->mTextureCoords[0][i];
			tempTex.x = texture.x;
			tempTex.y = texture.y;

			vert = mesh->mTangents[i];
			tempTan.x = vert.x;
			tempTan.y = vert.y;
			tempTan.z = vert.z;


			//if fails, default to 0,0 coords
 		}
		else
		{
			tempTex = glm::vec2(0,0);
			tempTan = glm::vec3(0.0);
		}

		//push into object
		final.addVert(Vertex(tempVec, tempColor, tempTex, tempTan));
	}

	//load indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		face = mesh->mFaces[i];

		for (int c = 0; c < face.mNumIndices; c++)
		{
			final.addIndice(face.mIndices[c]+1);
		}
	}

	mesh = NULL;
	scene = NULL;

	inputObj = final;
	return true;
}

bool loader::loadShader(string filename, string& output) {
	isEOF = false;
	string final = "";
	char temp;
	string finalFile = rootDir + "shaders/" +filename;
	filein.open(finalFile.c_str());

	if (filein.fail())
	{
		//read failed, return empty object
		cout << "FILE " << finalFile << " UNABLE TO BE READ" << endl;
		return false;
	}

	//load file
	while(!isEOF) {
		temp = filein.get();
		if (filein.eof())
		{
			isEOF = true;
			//throw away EOF bit
		} else
		{
			final += temp;
		}
	}

	//return file
	output = final;

	filein.close();
	return true;
}

bool loader::loadTexture(string filename, Texture &output)
{
	Magick::Blob blob;
	Magick::Image *tex;
	string finalFile = rootDir + "textures/" + filename;

	// load texture
	try
	{
		tex = new Magick::Image(finalFile);
		tex->write(&blob, "RGBA");
	}
	catch (Magick::Error& Error)
	{
		cout << "Error loading Texture " << finalFile << ": " << Error.what() << endl;
		return false;
	}

	//extract data
	output = Texture(tex->rows(), tex->columns(), blob.data());
	return true;
}

void loader::init()
{
	//initialize root directory
	loader test;
	if (test.loadModel("cube.obj").IB == 0)
	{
		rootDir = "../assets/";
		cout << "ROOT CHANGED" << endl;
	}
}

void loader::end()
{
	//clear all textures and models
	for (int i = 0; i < modelBank.size(); i++)
	{
		glDeleteBuffers(1, &modelBank[i].VB);
		glDeleteBuffers(1, &modelBank[i].IB);
	}

	for (int i = 0; i < textureBank.size(); i++)
	{
		glDeleteTextures(1, &textureBank[i].texture);
	}

	modelBank.clear();
	textureBank.clear();
}

ModelInstance loader::loadModel(string filename)
{
	return pushModel(filename);
}
TexInstance loader::loadTexture(string filename)
{
	return loadTexture(filename, GL_TEXTURE0);
}

TexInstance loader::loadTexture(string filename, GLenum position)
{
	return pushTexture(filename, position);
}

TexInstance loader::loadCubemap(string filename)
{
	return pushTexture(filename, GL_TEXTURE3, GL_TEXTURE_CUBE_MAP);
}

ModelInstance loader::pushModel(string filename)
{
	//search for model
	for (int i = 0; i < modelBank.size(); i++)
	{
		if (modelBank[i].name.compare(filename) == 0)
		{
			//if exists, return pointer
			return modelBank[i];
		}
	}

	//model doesn't exist yet, add
	  obj object;
	  ModelInstance final;
	  if (loadObject(filename, object))
	  {
		  GLuint tempVB;
		  GLuint tempIB;

		  //model loading
		  glGenBuffers(1, &tempVB);
		  glBindBuffer(GL_ARRAY_BUFFER, tempVB);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (object.getVerts().size()+1), &object.getVerts()[0], GL_STATIC_DRAW);

		  glGenBuffers(1, &tempIB);
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempIB);
		  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object.getIndices().size(), &object.getIndices()[0], GL_STATIC_DRAW);

		  //add to model bank
		  final = ModelInstance(tempVB, tempIB, object.getIndices().size(), filename);
		  modelBank.push_back(final);

		  //return
		  return final;
	  }

	//error, return empty vector
	return final;
}

TexInstance loader::pushTexture(string filename, GLenum position, GLenum type)
{
	//search if texture already exists
	for (int i  = 0; i < textureBank.size(); i++)
	{
		if (textureBank[i].name.compare(filename) == 0)
		{
			//if exists, return pointer
			return textureBank[i];
		}
	}

	//texture doesn't exist yet, check type and add in
	if (type == GL_TEXTURE_2D)
	{
		  Texture texture;

		  if (loadTexture(filename, texture))
		  {
			  GLuint final;
			  //texture loaded
			  glGenTextures(1, &final);
			  bindTex(final, position);
			  setTex(texture);
			  glGenerateMipmap(GL_TEXTURE_2D);
			  textureBank.push_back(TexInstance(final, filename));
			  return TexInstance(final, filename);
		  }

	}
	else if (type == GL_TEXTURE_CUBE_MAP)
	{
		  Texture texture;

		  //attempt to load all textures
		  //extract base name
		  string baseName = filename.substr(filename.find_last_of("\\/")+1, filename.length());
		  if (loadTexture(filename + "/" + baseName + "0.jpg" , texture))
		  {
			  GLuint final;
			  Texture finalTex[6];
			  //load successful, load rest of the images
			  glGenTextures(1, &final);
			  bindTex(final, position);

			  setTex(texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
			  loadTexture(filename + "/" + baseName + "1.jpg" , texture);
			  setTex(texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
			  loadTexture(filename + "/" + baseName + "2.jpg" , texture);
			  setTex(texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
			  loadTexture(filename + "/" + baseName + "3.jpg" , texture);
			  setTex(texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
			  loadTexture(filename + "/" + baseName + "4.jpg" , texture);
			  setTex(texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
			  loadTexture(filename + "/" + baseName + "5.jpg" , texture);
			  setTex(texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
			  //setCubeMap(finalTex[0],finalTex[1],finalTex[2],finalTex[3],finalTex[4],finalTex[5]);
			  textureBank.push_back(TexInstance(final, filename));
			  //check for completeness
			  // Get any errors from OpenGL
			  auto error = glGetError();
			  if ( error != GL_NO_ERROR )
			  {
			    std::cerr<< "Error in CubeMap generation: " << error << endl;
			  }

			  return TexInstance(final, filename);
		  }
	}
	else
	{
		cerr << "ERROR LOADING TEXTURE, INVALID TEXTURE TYPE" << endl;
	}

	  //error, return 0
	  return TexInstance(0, "NULL");
}

TexInstance loader::pushTexture(string filename,GLenum position)
{
	return pushTexture(filename, position, GL_TEXTURE_2D);
}

void loader::setTex(Texture texture)
{
	  setTex(texture, GL_TEXTURE_2D);
}

void loader::setTex(Texture texture, GLenum type)
{
	  glTexImage2D(type, 0, GL_RGBA, texture.column, texture.row, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	  if (type == GL_TEXTURE_2D)
	  {
		  glTexParameterf(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameterf(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  }
	  else
	  {
		  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	  }

}

void loader::bindTex(GLuint &bind, GLenum unit)
{
	  bindTex(bind, unit, GL_TEXTURE_2D);
}

void loader::bindTex(GLuint &bind, GLenum unit, GLenum type)
{
	  glActiveTexture(unit);
	  glBindTexture(type, bind);
}

string loader::getRoot()
{
	return rootDir;
}
