/*
 * loader.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#include <loader.h>

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
	aiFace face;
	glm::vec3 tempVec;
	glm::vec3 tempColor;
	int indice;

	//load object from file
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename,aiProcessPreset_TargetRealtime_Fast);

	//if fail, throw error
	if (scene == NULL) {
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
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

		//push into object
		final.addVert(Vertex(tempVec, tempColor));
	}

	//load indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		face = mesh->mFaces[i];

		//made to only work with triangles
		final.addIndice(face.mIndices[0]);
		final.addIndice(face.mIndices[1]);
		final.addIndice(face.mIndices[2]);
	}

	delete mesh;
	delete scene;

	mesh = NULL;
	scene = NULL;

	inputObj = final;
	return true;
}

bool loader::loadShader(string filename, string& output) {
	isEOF = false;
	string final = "";
	char temp;
	filein.open(filename.c_str());

	if (filein.fail())
	{
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
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


/*========
 * DEPRECATED
 *
 * All these functions below are deprecated
 * assimp now handles all the file loading
 * =======
 */

void loader::readLine(fstream &file, char &current, obj &object)
{
	switch (current)
	{
	case '#':
		readComment(file, current, object);
		break;
	case 'o':
		readObj(file, current, object);
		break;
	case 'v':
		if (file.peek() == ' ') {
			readVertex(file, current, object);
		}
		else if (file.peek() == 'n') {
			file.get();
			readUnknown(file, current, object);
		}
		break;
	case 's':
		readSection(file, current, object);
		break;
	case 'f':
		readIndice(file, current, object);
		break;
	default:
		readUnknown(file, current, object);
		break;
	}
}

void loader::readComment(fstream &file, char &current, obj &object)
{
	char read;
	//ignore everything
	while (true)
	{
		read = file.get();
		if (isAnnoying)
			cout << read;

		if (read == '\n' || read == '\r') {
			break;
		}

		if (read == -1) {
			isEOF = true;
			break;
		}
	}
	if (isAnnoying)
		cout << endl;
	current = file.get();
}

void loader::readObj(fstream &file, char &current, obj &object)
{
	string read;
	//assume it has no spaces
	//currently does nothing
	file >> read;
	if (isAnnoying)
		cout << read << endl;

	//next char
	file >> read;
	current = read.c_str()[0];
}

void loader::readVertex(fstream &file, char &current, obj &object)
{
	glm::vec3 vect;
	float reader;
	//read in three float values
	file >> vect.x;
	file >> vect.y;
	file >> vect.z;

	//build vertex and add
	object.addRaw(vect);
	if (isAnnoying)
		cout << vect.x << "," << vect.y << "," << vect.z << endl;

	//load next
	file >> current;
}

void loader::readSection(fstream &file, char &current, obj &object)
{
	string read;
	//assume it has no spaces
	//currently does nothing
	file >> read;
	if (isAnnoying)
		cout << read << endl;
	//next char
	file >> current;
}

void loader::readIndice(fstream &file, char &current, obj &object)
{
	string readin;
	unsigned int *indSet = NULL;
	for (int i = 0; i < 3; i++)
	{
		//read indice set
		file >> readin;
		indSet = getIndices(readin);

		//read in first number

		object.addIndice(indSet[0]);

		if (isAnnoying)
			cout << indSet[0] << ",";

		delete indSet;
		indSet = NULL;
	}
	if (isAnnoying)
		cout << endl;

	//next char
	if (current == -1)
		isEOF = true;
	else
		while (true)
		{
			current = file.get();
			if (current >= 'A' && current <= 'z')
				break;
			else if (current == -1)
			{
				isEOF = true;
				break;
			}
		}
}

unsigned int* loader::getIndices(string parse)
{
	unsigned int *final = new unsigned int[3];

	//extract first number
	string number = "";
	int stringIndex = 0;
	for (int i = 0; i < 3; i++) {
		//reset
		number = "";

		while (parse[stringIndex] >= '0' && parse[stringIndex] <= '9')
		{
			//get all chars that make up the number
			number += parse[stringIndex];
			stringIndex++;
		}
		//move to start of next number
		stringIndex++;

		//if there is no number, default to 0
		if (number.compare("") == 0)
			number = "0";

		//parse string to int
		final[i] = atoi(number.c_str());
	}

	return final;
}

void loader::readUnknown(fstream &file, char &current, obj &object)
{
	//skip line
	if (isAnnoying)
		cout << "[UNKNOWN]: ";
	while (true)
	{
		current = file.get();
		if (isAnnoying)
			cout << current;
		if (current == -1)
		{
			isEOF = true;
			break;
		}

		if (current == '\n')
		{
			current = file.get();
			break;
		}
	}

	if (isAnnoying)
		cout << endl;
}

void loader::calculateNormals(obj &object)
{
	glm::vec3 vert1;
	glm::vec3 vert2;
	glm::vec3 vert3;
	glm::vec3 final;

	Vertex *temp = NULL;

	vector<unsigned int> indices = object.getIndices();
	vector<glm::vec3> vertices = object.getRawVerts();
	//iterate through all faces and build vertex normals
	for (int i = 0; i < indices.size(); i += 3) {
		//load all three vertices
		//get indices-1 to get the correct vertex index

		if (isAnnoying)
			cout << "loaded" << indices[i] << indices[i+1]<< indices[i+2] << endl;
		vert1 = vertices[indices[i]];
		vert2 = vertices[indices[i+1]];
		vert3 = vertices[indices[i+2]];

		//calculate face normal
		final = glm::cross(vert2 - vert1, vert3 - vert1);
		final = glm::normalize(final);
		if (isAnnoying)
			cout << final.x << " " << final.y << " " << final.z << endl;

		//add to all affected vertices
		//normalize with any existing normals
		if (isAnnoying)
			cout << "built vert at" << indices[i] << endl;

		temp = new Vertex(vert1, final);
		object.addVert(*temp, indices[i]);
		delete temp;

		temp = new Vertex(vert2, final);
		object.addVert(*temp, indices[i+1]);
		delete temp;

		temp = new Vertex(vert3, final);
		object.addVert(*temp, indices[i+2]);
		delete temp;
		temp = NULL;
	}
}

bool loader::isHeader(string test)
{
	char extracted = test.c_str()[0];

	switch (extracted)
	{
	case '#':
	case 'o':
	case 'v':
	case 's':
	case 'f':
		return true;
	default:
		return false;
	}
}
