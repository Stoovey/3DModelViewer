//Include files to acccess libraries and to also run the project.
#include <SDL.h>
#include "glew.h"
#include "Scene.h"
#include <iostream>
#include <string>
#undef main
using namespace std;



//Initialize openGL, If we get an error display this in the console
bool InitGL()
{
	//Enable GlewExperimental due to it having problems loading core OpenGL :(
	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "ERROR: GLEW has failed to Initialize with error message: " << glewGetErrorString(err) << endl;
		return false;
	}
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	//Debug some information to the screen regarding what versions of libraries we are using!
	//Being up-to-date is good!

	cout << "INFO: GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
	cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "INFO: OpenGL Shader Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
	//end of Debug info

	return true;
}

int main(int argc, int *argv[])
{
	long last = 0;
	float deltaTime = 0.0;
	float x, y;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "ERROR: SDL cannot Initialize. Please view logs" << endl;
		return -1;
	}
	//If SDL Initializes Set Attributes for SDL/GL linkage
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

	//Open windows
	int winPosX = 100;
	int winPosY = 100;
	int winWidth = 600;
	int winHeight = 600;
	SDL_Window *window = SDL_CreateWindow("3D Model Viewer - Steven Reynolds", winPosX, winPosY, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_Renderer * renderer =  SDL_CreateRenderer (window, -1, 0);
	SDL_GLContext glcontext = SDL_GL_CreateContext (window);
	//End Open windows

	//Another call for GL to open
	if (!InitGL())
	{
		return -1;
	}
	unsigned int lastTime = SDL_GetTicks();
	glEnable(GL_DEPTH_TEST);

	Scene currentScene;

	//Wow, we managed to get here, now lets run the application

	bool startUpApplication = true;
	float mouseInputX = 1.0f, mouseInputY = 1.0f;

	while (startUpApplication)
	{
		long currentFirst = SDL_GetTicks();

		if (currentFirst > last)
		{
			deltaTime = ((float)(currentFirst - last)) / 1000;
			last = currentFirst;
		}

		float move = 1.0f * deltaTime;
		SDL_Event incomingEvent;

		while(SDL_PollEvent (&incomingEvent ))
		{
			switch (incomingEvent.type)
			{
			case SDL_QUIT:
				startUpApplication = false;
				break;
			case SDL_MOUSEMOTION:
				x = incomingEvent.motion.x;
				y = incomingEvent.motion.y;

				mouseInputX = x;
				mouseInputY = y;
				break;
			case SDL_KEYDOWN:
				switch(incomingEvent.key.keysym.sym)
				{
				case  SDLK_ESCAPE:
					startUpApplication = false;
					break;
				}

			}
			break;
		} 


		unsigned int current = SDL_GetTicks();
		float deltaTime = (float) ( current - lastTime) / 1000.0f;
		lastTime = current;
		//While we are at the current scene, update in Delta time
		//The mouseInput of X,Y to rotate the Model.
		currentScene.Update(deltaTime, mouseInputX, mouseInputY);
		//Clear the colors
		glClearColor(0.5f, 0.5f, 0.5f, 0.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentScene.Draw();
		SDL_GL_SwapWindow(window);

		if(deltaTime < (1.0f/50.0f) )
		{
			SDL_Delay((unsigned int) (((1.0f/50) - deltaTime) * 1000.0f));
		}

	}

	//When the User quits the program destroy everything.
	SDL_GL_DeleteContext (glcontext);
	SDL_DestroyWindow (window);
	SDL_Quit();
	return 0;

	//User has quit the program now.

}