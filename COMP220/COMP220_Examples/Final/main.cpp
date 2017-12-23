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
		"Project",
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

	GameObject * armouredTank = new GameObject();
	armouredTank->loadMesh("Tank1.FBX");
	armouredTank->loadDiffuseMap("Tank1DF.png");
	armouredTank->setPosition(0.0f, -3.0f, 0.0f); 
	armouredTank->setRotation(0.0f, 0.7f, 0.0f);
	armouredTank->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");

	Camera * firstPersonCamera = new Camera();


	// Light
	vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightDirection = vec3(0.0f, 0.0f, -1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specularLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Material
	vec4 ambientMaterialColour = vec4(0.05f, 0.05f, 0.05f, 1.0f);
	vec4 diffuseMaterialColour = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	vec4 specularMaterialColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularPower = 25.0f;

	// Colour Buffer Texture
	GLuint colourBufferID = createTexture(800, 800);

	// Create Depth Buffer
	GLuint depthRenderBufferID;
	glGenRenderbuffers(1, &depthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 800, 800);

	// Create framebuffer
	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferID); // attach buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourBufferID,0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unable to create frame buffer for post processing", "Frame Buffer Error", NULL);
	}

	// Create screen aligned quad
	GLfloat screenVerts[] =
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};

	GLuint screenQuadVBOID;
	glGenBuffers(1, &screenQuadVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), screenVerts, GL_STATIC_DRAW);

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint postProcessingProgramID = LoadShaders("passThroughVert.glsl", "postBlackAndWhite.glsl");
	GLint texture0Location = glGetUniformLocation(postProcessingProgramID, "texture0");

	// Create and compile our GLSL program from the shaders
	//GLint programID = LoadShaders("lightingVert.glsl", "lightingFrag.glsl");

	// sets frag colour
	static const GLfloat fragColour[] = { 1.0f,1.0f,1.0f,1.0f };
	
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
				firstPersonCamera->setRotation(ev.motion.xrel, ev.motion.yrel);
				break;

			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					// Keys
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_w:
					firstPersonCamera->moveCameraForward(0.2f);
					break;
				case SDLK_s:
					firstPersonCamera->moveCameraForward(-0.2f);
					break;
				case SDLK_a:
					firstPersonCamera->moveCameraRight(-0.5f);
					break;
				case SDLK_d:
					firstPersonCamera->moveCameraRight(0.5f);
					break;


				}

				firstPersonCamera->update();
			}

		}
		//updates view matrix
		firstPersonCamera->updateViewMatrix();

		// Gets current tick
		currentTicks = SDL_GetTicks();

		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;
		
		armouredTank->update();

		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		//Update Game and Draw with OpenGL!
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		armouredTank->preRender();

		GLuint currentShaderProgramID = armouredTank->getShaderProgramID();

		GLint viewMatrixLocation = glGetUniformLocation(currentShaderProgramID, "viewMatrix");
		GLint projectionMatrixLocation = glGetUniformLocation(currentShaderProgramID, "projectionMatrix");
		GLint camerPositionLocation = glGetUniformLocation(currentShaderProgramID, "cameraPosition");

		GLint lightDirectionLocation = glGetUniformLocation(currentShaderProgramID, "lightDirection");
		GLint ambientLightColourLocation = glGetUniformLocation(currentShaderProgramID, "ambientLightColour");
		GLint diffuseLightColourLocation = glGetUniformLocation(currentShaderProgramID, "diffuseLightColour");
		GLint specularLightColourLocation = glGetUniformLocation(currentShaderProgramID, "specularLightColour");


		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(firstPersonCamera->getViewMatrix()));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(firstPersonCamera->getProjectionMatrix()));
		glUniform3fv(camerPositionLocation, 1, value_ptr(firstPersonCamera->getCameraPosition()));

		glUniform3fv(lightDirectionLocation, 1, value_ptr(lightDirection));
		glUniform4fv(ambientLightColourLocation, 1, value_ptr(ambientLightColour));
		glUniform4fv(diffuseLightColourLocation, 1, value_ptr(diffuseLightColour));
		glUniform4fv(specularLightColourLocation, 1, value_ptr(specularLightColour));

		armouredTank->render();
		
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1, 0.2, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind Post Processing Shaders
		glUseProgram(postProcessingProgramID);

		//Activate texture unit 0 for the colour buffer
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colourBufferID);
		glUniform1i(texture0Location, 0);

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//Updates display
		SDL_GL_SwapWindow(window);
	}

	////delete dynamics world
	//delete dynamicsWorld;

	////delete solver
	//delete solver;

	////delete broadphase
	//delete overlappingPairCache;

	////delete dispatcher
	//delete dispatcher;

	//delete collisionConfiguration;


	glDeleteProgram(postProcessingProgramID);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenQuadVBOID);
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteRenderbuffers(1, &depthRenderBufferID);
	glDeleteTextures(1, &colourBufferID);

	if (armouredTank)
	{
		armouredTank->destroy();
		delete armouredTank;
		armouredTank = nullptr;
	}

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
