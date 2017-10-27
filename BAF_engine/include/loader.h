/*
 * loader.h
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <iostream>
#include <fstream>
#include "obj.h"

#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects
#include <Magick++.h>

struct ModelInstance {
	GLuint VB;
	GLuint IB;
	unsigned int size;
	string name;
	ModelInstance(GLuint v, GLuint i, unsigned int s, string n): VB(v), IB(i), size(s), name(n){}
	ModelInstance(): VB(0), IB(0), size(0), name("NULL"){}
};

struct TexInstance {
	GLuint texture;
	string name;
	TexInstance(GLuint m, string n): texture(m), name(n){}
	TexInstance(): texture(0), name("NULL"){};
};

using namespace std;

class loader {
private:
	fstream filein;
	bool isEOF;
	bool isAnnoying;

    static vector<ModelInstance> modelBank;
    static vector<TexInstance> textureBank;
    static string rootDir;

    TexInstance pushTexture(string, GLenum);
    TexInstance pushTexture(string, GLenum, GLenum);
    ModelInstance pushModel(string);
    void setTex(Texture);
    void setTex(Texture texture, GLenum type);
    void bindTex(GLuint&, GLenum);
    void bindTex(GLuint&, GLenum, GLenum);

public:
	loader();
	~loader();

	static void init();
	static void end();
	bool loadTexture(string, Texture&);
	bool loadObject(string, obj&);
	bool loadShader(string, string&);
	ModelInstance loadModel(string);
	TexInstance loadTexture(string);
	TexInstance loadTexture(string, GLenum);
	TexInstance loadCubemap(string);
	string getRoot();
};

#endif /* LOADER_H_ */
