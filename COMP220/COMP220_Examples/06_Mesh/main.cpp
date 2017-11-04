//main.cpp - defines the entry point of the application

#include "main.h"

int main(int argc, char* args[])
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow(
		"Triangle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		800,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//Request 3.2 Core OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext gl_Context = SDL_GL_CreateContext(window);
	if (gl_Context == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateContext Failed", SDL_GetError(), NULL);

		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GLEW Initialisation failed", (char*)glewGetErrorString(err), NULL);

		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	std::vector<Mesh*> meshes;
	loadMeshesFromFile("Tank1.FBX", meshes);
	GLuint textureID = loadTextureFromFile("Tank1DF.png");

	loadMeshesFromFile("cube.nff", meshes);
	GLuint textureID2 = loadTextureFromFile("crate.jpg");

	vec3 trianglePosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 triangleScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 triangleRotation = vec3(0.0f, 0.0f, 0.0f);

	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(triangleScale);
	mat4 rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));

	vec3 cameraPosition = vec3(0.0f, 0.0f, -5.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraDirection = vec3(0.0f, 0.0f, 0.0f);
	float cameraDistance = 5.0;

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);
	mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;
	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);

	// Create and compile our GLSL program from the shaders
	GLint programID = LoadShaders("textureVert.glsl", "textureFrag.glsl");

	// sets frag colour
	static const GLfloat fragColour[] = { 1.0f,1.0f,1.0f,1.0f };
	
	// links 
	GLint location = glGetUniformLocation(programID, "fragColour");
	GLint currentTimeLocation = glGetUniformLocation(programID, "time");
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");

	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();

	SDL_SetRelativeMouseMode(SDL_TRUE);

	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;
				//KEYDOWN Message, called when a key has been pressed down

			case SDL_MOUSEMOTION:

				cameraDirection = normalize(cameraTarget - cameraPosition);
		
				cameraDirection.x -= (float (ev.motion.xrel) /100);
				cameraDirection.y -= (float(ev.motion.yrel) /100);

				//printf("%f , %f, %f  \n", cameraDirection.x, cameraDirection.y, cameraDirection.z);

				// distance
				cameraTarget = (cameraPosition + (cameraDirection * 5.0f));
				break;


			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_RIGHT:
					cameraPosition -= cross(cameraUp, cameraDirection);
					break;
				case SDLK_LEFT:
					cameraPosition += cross(cameraUp, cameraDirection);
					break;
				case SDLK_UP:
					cameraPosition += (cameraDirection * 0.5f);
					break;
				case SDLK_DOWN:
					cameraPosition -= (cameraDirection * 0.5f);
					break;
				}
				cameraTarget = (cameraPosition + (cameraDirection * 5.0f));
			}
		}

		mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

		// Gets current tick
		currentTicks = SDL_GetTicks();

		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		//Update Game and Draw with OpenGL!
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);


		// Use our shader
		glUseProgram(programID);

		glUniform4fv(location, 1, fragColour);
		glUniform1f(currentTimeLocation, (float)(currentTicks) / 1000.0f);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniform1i(textureLocation, 0);

		for (Mesh * currentMesh : meshes)
		{
			currentMesh->render();
		}

		//Updates display
		SDL_GL_SwapWindow(window);
	}


	//out of loop
	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			(*iter)->destroy();
			delete (*iter);
			iter = meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	meshes.clear();


	// Deletes the GLSL program created earlier
	glDeleteProgram(programID);
	glDeleteTextures(1, &textureID);


	//Delete Context
	SDL_GL_DeleteContext(gl_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	
	IMG_Quit();
	SDL_Quit();

	return 0;
}

//copy ..\..\..\Libraries\SDL2_image - 2.0.1\lib\x64\*.dll "$(OutDir)*.dll"
//copy ..\..\..\Libraries\SDL2 - 2.0.6\lib\x64\SDL2.dll "$(OutDir)SDL2.dll"
//copy ..\..\..\Libraries\glew - 2.1.0\bin\Release\x64.glew32.dll "$(OutDir)glew32.dll"