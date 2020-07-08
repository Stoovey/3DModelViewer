#include "Model.h"
#include <string>
#include <vector>
#include <glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
Model::Model()
{
	_VAO = 0;
	glGenVertexArrays( 1, &_VAO );
	glBindVertexArray( _VAO );
	
	string LineBeginning;
	string dummyLine;
	string objectName;

	//temp stuff
	vector<glm::vec3> Faces;
	vector<glm::vec3> Normals;
	vector<glm::vec2> Texture;
	vector<glm::vec3> VertexData;
	
	int fCount = 0;
	int vnCount = 0;
	int vtCount = 0;
	int vCount = 0;
LOOP:
	cout << "Welcome to my 3D Model Viewer. " << endl;
	cout << "Please put a obj in the working directory" << endl;
	cout << "Enter the file name only not with the .obj" << endl;
	cout << "Name of file you want to open:"; cin >> objectName;
	

	objectName = objectName + ".obj";

	ifstream textFile;
	textFile.open(objectName);
	if (textFile.fail())
	{
		cout << "Error loading file" << endl;
		system("cls");
		goto LOOP;
	}
	else
	{
		while (!textFile.eof())
		{
			if(textFile >> LineBeginning)
			{
				if(LineBeginning == "v")
				{
					glm::vec3 tempvec3;
					textFile >> tempvec3.x >> tempvec3.y >> tempvec3.z;
					VertexData.push_back(tempvec3);
					vCount++;
				}
				else if(LineBeginning == "vt")
				{
					glm::vec2 tempvec2;
					textFile >> tempvec2.x >> tempvec2.y;
					Texture.push_back(tempvec2);
				}
				else if(LineBeginning == "vn")
				{
					glm::vec3 tempvec1;
					textFile >> tempvec1.x >> tempvec1.y >> tempvec1.z;
					Normals.push_back(tempvec1);
				}
				else if(LineBeginning == "f") 
				{
					fCount++;

					string faced, 
						   faced2, 
						   faced3;
					textFile >> faced >> faced2 >> faced3;
					istringstream twitch1(faced);
					istringstream twitch2(faced2);
					istringstream twitch3(faced3);
					
					string  val, 
							val2, 
							val3;
					getline(twitch1, val, '/');
					getline(twitch2, val2, '/');
					getline(twitch3, val3, '/');
					int pos = atoi (val.c_str());
					int pos2 = atoi (val2.c_str());
					int pos3 = atoi (val3.c_str());
					if (pos > 0 && pos2 > 0 && pos3 > 0)
					{
						Faces.push_back (VertexData.at(pos - 1));
						Faces.push_back (VertexData.at(pos2 - 1));
						Faces.push_back (VertexData.at(pos3 - 1));
					}
				}
				//dummyLine is used to skip to the next line
				//instead of reading each individual word within the file.
				getline(textFile, dummyLine);
			}
			else
			{
				cout << "End of File" << endl;
			}
		}
	}
	textFile.close(); 

	// Number of vertices in above data
	_objectVertices = Faces.size();
	_objectTextures = Texture.size();
	_objectNormals = Normals.size();

	cout << "\nNumber of Vertices: " << vCount;
	cout << "\nNumber of Triangles: " << fCount << endl;

	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _objectVertices * 3, Faces.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);

	GLuint normalBuffer = 0;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _objectNormals * 3, Normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(1);

	GLuint texcoordBuffer = 0;
	glGenBuffers(1, &texcoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _objectTextures * 2, Texture.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );

	glDisableVertexAttribArray(0);
}

Model::~Model()
{
	glDeleteVertexArrays( 1, &_VAO );
}

void Model::Draw()
{
	glBindVertexArray( _VAO );
	glDrawArrays(GL_TRIANGLES, 0, _objectVertices);
	glBindVertexArray( 0 );
}