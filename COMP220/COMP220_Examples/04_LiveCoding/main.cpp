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

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Vertex triangleVertices[] =
	{
		{ -0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f },
		{ 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f },
		{ 0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f },
		{ -0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f }
	};

	unsigned int triangleIndices[] =
	{
		0,1,2,
		2,0,3
	};

	//This will identify our vertex buffer
	GLuint vertexbuffer;
	//Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	//The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), triangleVertices, GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), triangleIndices, GL_STATIC_DRAW);

	GLuint textureID = loadTextureFromFile("Crate.jpg");

	vec3 trianglePosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 triangleScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 triangleRotation = vec3(0.0f, 0.0f, 0.0f);

	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(triangleScale);
	mat4 rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));

	mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;

	vec3 cameraPosition = vec3(0.0f, 0.0f, -5.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

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

	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();

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
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}
		// Gets current tick
		currentTicks = SDL_GetTicks();

		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		//Update Game and Draw with OpenGL!
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// changes the colour of frag shader
		//fragColour[2] = (currentTimeLocation, (float)(currentTicks) / 10000.0f);


		//Buffers
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                 //attribute 0, No particular reason for 0, but must match the layout in the shader.
			3,                 //size
			GL_FLOAT,          //type
			GL_FALSE,          //normalised?
			sizeof(Vertex),    //stride
			(void*)0           //array buffer offset
		);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		//glDisableVertexAttribArray(2);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));


		//Draw the triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		//Updates display
		SDL_GL_SwapWindow(window);
	}

	// Deletes the GLSL program created earlier
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &elementbuffer);
	//glDeleteTextures(1, &textureID);
	glDeleteVertexArrays(1, &VertexArrayID);

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


//copy ..\..\..\Libraries\SDL2 - 2.0.6\lib\x64\SDL2.dll "$(OutDir)SDL2.dll"
//copy ..\..\..\Libraries\glew - 2.1.0\bin\Release\x64.glew32.dll "$(OutDir)glew32.dll"