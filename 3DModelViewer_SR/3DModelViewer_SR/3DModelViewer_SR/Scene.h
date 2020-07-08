
//Include of the main GLM library that allows us to use vector and matrix functions that will help us to work with OpenGL

#include "glm.hpp"
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Model.h"
#include <SDL.h>
#include <string>


class Scene 
{
public:

	//Constructor and Deconstructure
	Scene();
	~Scene();

	//Following functions are getting the float values, and then storing them for use.
	void moveCameraAngleX(float value) { _cameraAngleX += value;}
	void moveCameraAngleY(float value) { _cameraAngleY += value;}

	//Update: Updating axixs of the Horizontal and Vertical for the mouse agaisnt deltatime.
	void Update (float deltaTime, int mouseInputX, int mouseInputY);

	void Draw();
	unsigned int LoadTexture( std::string filename );

protected:

	//Rotations of Model
	float _cameraAngleX, _cameraAngleY;
	float _modelAngle;
	unsigned int _modelTexture;

	//Projection Matrix's
	Model _objectModel;
	glm::mat4 _modelMatrix;
	glm::mat4 _modelViewMatrix;
	glm::mat4 _modelProjMatrix;

	//Begin Shader Code -> Future work to implement this into it's own class.
	GLuint _shaderProgram;

	int _shaderModelMatLocation;
	int _shaderViewMatLocation;
	int _shaderProjMatLocation;

	int _shaderDiffuseColLocation, _shaderEmissiveColLocation;
	int _shaderWSLightPosLocation;

	int _shaderTexSamplerLocation;

	bool CheckShaderCompiled( GLint shader );
	void BuildShaders();

	

};



