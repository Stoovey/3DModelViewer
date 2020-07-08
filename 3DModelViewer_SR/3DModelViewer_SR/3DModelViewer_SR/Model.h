#ifndef __MODEL_H__
#define __MODEL_H__

#include <SDL.h>
#include "glew.h"


class Model
{
public:
	//Create Constructors and Deconstructors
	Model();
	~Model();

	void Draw();

	int _objectVertices;
	int _objectTextures;
	int _objectNormals;
	int _objectFaces;

protected:
	GLuint _VAO;
};

#endif