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
#include <obj.h>

using namespace std;

class loader {
private:
	fstream filein;
	bool isEOF;
	bool isAnnoying;

	void readLine(fstream&, char&, obj&);
	void readComment(fstream&, char&, obj&);
	void readObj(fstream&, char&, obj&);
	void readVertex(fstream&, char&, obj&);
	void readSection(fstream&, char&, obj&);
	void readIndice(fstream&, char&, obj&);
	void readUnknown(fstream&, char&, obj&);

	void calculateNormals(obj&);

	bool isHeader(string);
	unsigned int* getIndices(string parse);
public:
	loader();
	~loader();

	bool loadObject(string, obj&);
	bool loadShader(string, string&);

};

#endif /* LOADER_H_ */
