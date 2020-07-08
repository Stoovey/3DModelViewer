#include "Scene.h"
#include <iostream>
#include <SDL.h>
using namespace std;
SDL_Event event;

Scene::Scene()
{
	//Initialize all of our Scene constructors 

	//Model and Camera
	_modelAngle = 0.0f;
	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;

	//Initialize our shaders

	_shaderModelMatLocation = 0;
	_shaderViewMatLocation = 0;
	_shaderProjMatLocation = 0;
	_shaderDiffuseColLocation = 0;
	_shaderEmissiveColLocation = 0;
	_shaderWSLightPosLocation = 0;
	_shaderTexSamplerLocation = 0;

	//Build Shaders
	BuildShaders();

	//When everything is Initialized, Initialize Matrix for Viewing, the Projection and the Texture to Load.
	_modelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,-0.5f,-6.0f) );
	_modelProjMatrix = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
	//_modelTexture = LoadTexture("Image.bmp");
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime, int mouseInputX, int mouseInputY)
{
	_modelAngle += deltaTime * 0.5f;

	//Limit the loops the model can do
	while (_modelAngle > (3.14159265358979323846 * 2.0) )
	{
		_modelAngle -= (3.14159265358979323846 * 2.0);
	}

	//Update Model Matrix with the current rotation of the Object
	_modelMatrix = glm::rotate( glm::mat4(1.0f), _modelAngle, glm::vec3(mouseInputX,mouseInputY,0) );

}

void Scene::Draw()
{

	glUseProgram( _shaderProgram );
	glUniform1i(_shaderTexSamplerLocation,0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(_modelViewMatrix) );
	glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(_modelProjMatrix) );

	glUniform3f( _shaderEmissiveColLocation, 0.0f, 0.0f, 0.0f );

	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrix) );
	glUniform3f( _shaderDiffuseColLocation, 1.0f, 0.3f, 0.3f );
	_objectModel.Draw( );

	glUseProgram( 0 );
}


//Following function to see if our Shader has compiled, if not it will display an error through "log" in the console.
bool Scene::CheckShaderCompiled( GLint shader )
{
	GLint compiled;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled);
	if ( !compiled)
	{
		GLsizei len;
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &len);
		GLchar *log = new GLchar[len+1];
		cerr << "ERROR: Shader Compiler failed due to: " << log << endl;
		delete [] log;

		return false;
	}
	return true;


}

//If we do not fail the Shader Compile, build the shaders:) This will be in future a brand new class
//It's just to big to find what you need :(
void Scene::BuildShaders()
{
	// This is the vertex shader
	const GLchar *vShaderText = "#version 430 core\n\
								layout(location = 0) in vec4 vPosition;\n\
								layout(location = 1) in vec3 vNormalIn;\n\
								layout(location = 2) in vec2 vTexCoordIn;\n\
								\n\
								uniform mat4 modelMat;\n\
								uniform mat4 invModelMat;\n\
								uniform mat4 viewMat;\n\
								uniform mat4 projMat;\n\
								\n\
								uniform vec4 worldSpaceLightPos = {1,0.0,1,1};\n\
								\n\
								out vec3 vNormalV;\n\
								out vec3 eyeSpaceLightPosV;\n\
								out vec3 eyeSpaceVertPosV;\n\
								out vec2 texCoord;\n\
								\n\
								void main()\n\
								{\n\
								gl_Position = projMat * viewMat * modelMat * vPosition;\n\
								\n\
								eyeSpaceVertPosV = vec3(viewMat * modelMat * vPosition);\n\
								eyeSpaceLightPosV = vec3(viewMat * worldSpaceLightPos);\n\
								\n\
								vNormalV = mat3(viewMat * modelMat) * vNormalIn;\n\
								texCoord = vTexCoordIn;\n\
								}";
	// This is the fragment shader
	const GLchar *fShaderText = "#version 430 core\n\
								in vec3 vNormalV;\n\
								in vec3 eyeSpaceLightPosV;\n\
								in vec3 eyeSpaceVertPosV;\n\
								in vec2 texCoord;\n\
								\n\
								uniform vec3 lightColour = {0,1,0.5};\n\
								uniform vec3 emissiveColour = {0,0,0};\n\
								uniform vec3 ambientColour  = {0.1f,0.1f,0.2f};\n\
								uniform vec3 diffuseColour  = {0.8f,0.1f,0.1f};\n\
								uniform vec3 specularColour = {0.0f,1.0f,0.0f};\n\
								uniform float shininess     = 50.0f;\n\
								uniform float alpha         = 1.0f;\n\
								\n\
								out vec4 fragColour;\n\
								\n\
								uniform sampler2D tex1;\n\
								\n\
								void main()\n\
								{\n\
								vec3 lightDir = normalize( eyeSpaceLightPosV - eyeSpaceVertPosV );\n\
								vec3 normal = normalize( vNormalV );\n\
								\n\
								vec3 texColour = vec3(texture(tex1,texCoord));\n\
								vec3 diffuse = diffuseColour * lightColour * max(dot(normal,lightDir),0);\n\
								\n\
								vec3 specular = vec3(0,0,0);\n\
								\n\
								fragColour = vec4( emissiveColour + texColour + diffuse + specular, alpha);\n\
								}";
	_shaderProgram = glCreateProgram();
	GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vShader, 1, &vShaderText, NULL );
	glCompileShader( vShader );
	if( !CheckShaderCompiled( vShader ) )
	{
		std::cerr<<"ERROR: failed to compile vertex shader"<<std::endl;
		return;
	}
	glAttachShader( _shaderProgram, vShader );
	GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fShader, 1, &fShaderText, NULL );
	glCompileShader( fShader );
	if( !CheckShaderCompiled( fShader ) )
	{
		std::cerr<<"ERROR: failed to compile fragment shader"<<std::endl;
		return;
	}
	glAttachShader( _shaderProgram, fShader );
	glLinkProgram( _shaderProgram );
	GLint linked;
	glGetProgramiv( _shaderProgram, GL_LINK_STATUS, &linked );
	if ( !linked )
	{
		GLsizei len;
		glGetProgramiv( _shaderProgram, GL_INFO_LOG_LENGTH, &len );
		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( _shaderProgram, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;
		return;
	}
	glUseProgram( _shaderProgram );
	_shaderModelMatLocation = glGetUniformLocation( _shaderProgram, "modelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _shaderProgram, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _shaderProgram, "projMat" );

	_shaderDiffuseColLocation = glGetUniformLocation( _shaderProgram, "diffuseColour" );
	_shaderEmissiveColLocation = glGetUniformLocation( _shaderProgram, "emissiveColour" );
	_shaderWSLightPosLocation = glGetUniformLocation( _shaderProgram, "worldSpaceLightPos" );
	_shaderTexSamplerLocation = glGetUniformLocation( _shaderProgram, "tex1" );
}