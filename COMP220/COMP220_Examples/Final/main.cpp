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

	// Lists of game objects
	std::vector<GameObject*> gameObjectsList;
	std::vector<GameObject*> carsList;


	// spawn roads
	for (int w = 0; w < 2; ++w)
	{
		for (int i = 0; i < 4; ++i)
		{
			GameObject * road = new GameObject();
			road->getMaterial().loadMesh("Road.obj"); // created by poly by google https://poly.google.com/view/a38EEOJFUlp
			road->getMaterial().loadDiffuseMap("Road.png");
			road->getMaterial().loadShaderProgram("lightingVert.glsl", "lightingFrag.glsl");

			road->getObjectTransform().setPosition(i * -100.f, -10.f, w * 29.f);
			road->getObjectTransform().setScale(.5f, .5f, .5f);
			gameObjectsList.push_back(road);
		}
	}

	// spawn cars
	for (int i = 0; i < 4; ++i)
	{
		GameObject * carObject = new GameObject();
		carObject->getMaterial().loadMesh("Car.obj"); // created by poly by google https://poly.google.com/view/75h3mi6uHuC
		carObject->getMaterial().loadDiffuseMap("Car.png");
		carObject->getMaterial().loadShaderProgram("lightingVert.glsl", "lightingFrag.glsl");
	
		carObject->getObjectTransform().setPosition(-80.0f, -7.0f,(i * 14.5f) - 7.0f);
		carObject->getObjectTransform().setRotation(0.0f, 1.55f, 0.0f);
		carObject->getObjectTransform().setScale(0.2f, 0.2f, 0.2f);
		
		carObject->setHasPhysics(true);

		gameObjectsList.push_back(carObject);
		carsList.push_back(carObject);
	}

	// spaw player
	GameObject * motorcycleObject = new GameObject();
	motorcycleObject->getMaterial().loadMesh("motorcycle.obj"); // created by Poly by google https://poly.google.com/view/dse64pqMKAR
	motorcycleObject->getMaterial().loadDiffuseMap("motorcycle.png");
	motorcycleObject->getMaterial().loadShaderProgram("lightingVert.glsl", "lightingFrag.glsl");
	gameObjectsList.push_back(motorcycleObject);

	motorcycleObject->getObjectTransform().setPosition(0.0f, -7.0f, 15.0f);
	motorcycleObject->getObjectTransform().setRotation(0.0f, 1.55f, 0.0f);
	motorcycleObject->getObjectTransform().setScale(0.08f, 0.08f, 0.08f);
	motorcycleObject->setHasPhysics(true);

	// set player speed
	vec3 playerSpeed = vec3(0.f, -1.f, 0.f);
	// create camera
	Camera * firstPersonCamera = new Camera();
	// locks camera
	bool cameraLock = true;

	//for effects
	float effectCase = 0.;

	// Light
	vec4 ambientLightColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec3 lightDirection = vec3(-0.4f, 0.7f, 1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specularLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

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

	GLuint postProcessingProgramID = LoadShaders("passThroughVert.glsl", "postTextureFrag.glsl");
	GLint texture0Location = glGetUniformLocation(postProcessingProgramID, "texture0");

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	// set gravity
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// make ground
	PhysicsObject * groundShape = new PhysicsObject();
	groundShape->setCollisionShape(btScalar(50000.), btScalar(2.), btScalar(50.));
	groundShape->setObjectOrigin(0, -10, 0);
	groundShape->makeRigidBody();
	groundShape->addRigidBody(dynamicsWorld);

	// give cars physics
	for (GameObject * pObj : carsList)
	{
		pObj->getObjectPhysics().setCollisionShape(8, 2, 4);
		pObj->getObjectPhysics().setObjectOrigin(pObj->getObjectTransform().getPosition().x, pObj->getObjectTransform().getPosition().y, pObj->getObjectTransform().getPosition().z);
		pObj->getObjectPhysics().setObjectMass(1.);

		pObj->getObjectPhysics().makeRigidBody();
		pObj->getObjectPhysics().addRigidBody(dynamicsWorld);
		pObj->getObjectPhysics().randomiseSpeed();
	}

	// gives player physics
	motorcycleObject->getObjectPhysics().setCollisionShape(2, 2, 2);
	motorcycleObject->getObjectPhysics().setObjectOrigin(motorcycleObject->getObjectTransform().getPosition().x, motorcycleObject->getObjectTransform().getPosition().y, motorcycleObject->getObjectTransform().getPosition().z);
	motorcycleObject->getObjectPhysics().setObjectMass(1.);
	motorcycleObject->getObjectPhysics().makeRigidBody();
	motorcycleObject->getObjectPhysics().addRigidBody(dynamicsWorld);

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

					// Camera Controls
				case SDLK_UP:
					firstPersonCamera->moveCameraForward(2.f);
					break;
				case SDLK_DOWN:
					firstPersonCamera->moveCameraForward(-2.f);
					break;
				case SDLK_LEFT:
					firstPersonCamera->moveCameraRight(-5.f);
					break;
				case SDLK_RIGHT:
					firstPersonCamera->moveCameraRight(5.f);
					break;

					// Motorbike controls
				case SDLK_w:
					if (playerSpeed.x > -300.f)
						playerSpeed.x += -10.f;
					break;
				case SDLK_s:
					if (playerSpeed.x < 30.f)
						playerSpeed.x += 4.f;
					break;
				case SDLK_a:
					if (playerSpeed.z < 60.f)
						playerSpeed.z += 4.f;
					break;
				case SDLK_d:
					if (playerSpeed.z > -60.f)
						playerSpeed.z += -4.f;
					break;

					//effects
				case SDLK_SPACE:
					effectCase = 1.;
					break;
				case SDLK_p:
					effectCase = 2.;
					break;
				case SDLK_t:
					motorcycleObject->getMaterial().setAmbientMaterialColour(glm::vec4(.4f, .4f, .4f, .1f));
					motorcycleObject->getMaterial().setDiffuseMaterialColour(glm::vec4(.6f, .6f, .6f, .1f));
					motorcycleObject->getMaterial().setSpecularMaterialColour(glm::vec4(1.f, 1.f, 1.f, .1f));
					break;

					//reset
				case SDLK_r:
					effectCase = 0.;
					motorcycleObject->getMaterial().setAmbientMaterialColour(glm::vec4(.4f, .4f, .4f, 1.f));
					motorcycleObject->getMaterial().setDiffuseMaterialColour(glm::vec4(.6f, .6f, .6f, 1.f));
					motorcycleObject->getMaterial().setSpecularMaterialColour(glm::vec4(1.f, 1.f, 1.f, 1.f));
					break;

					//toggle camera lock
				case SDLK_y:
					if (cameraLock)
					{
						cameraLock = false;
					}
					else
					{
						cameraLock = true;
					}
					break;
				}

			}

		}

		// update speed and rotation of motorcycle
		motorcycleObject->getObjectTransform().setRotation(motorcycleObject->getObjectTransform().getRotation().x, motorcycleObject->getObjectTransform().getRotation().y, playerSpeed.z/100);
		motorcycleObject->getObjectPhysics().setObjectSpeed(playerSpeed.x, playerSpeed.y, playerSpeed.z);

		// lock camera if camera lock is true
		if (cameraLock)
		{
			firstPersonCamera->setCameraPoisition(motorcycleObject->getObjectTransform().getPosition().x + 15.f, motorcycleObject->getObjectTransform().getPosition().y + 10.f, motorcycleObject->getObjectTransform().getPosition().z);
			firstPersonCamera->setCameraTarget(motorcycleObject->getObjectTransform().getPosition().x, motorcycleObject->getObjectTransform().getPosition().y, motorcycleObject->getObjectTransform().getPosition().z);
		}

		// update view matrix
		firstPersonCamera->updateViewMatrix();

		// Get current tick
		currentTicks = SDL_GetTicks();

		GLint currentTimeLocation = glGetUniformLocation(postProcessingProgramID, "time");
		glUniform1f(currentTimeLocation, (float)(currentTicks) / 1000.f);
		
		GLint currentTestLocation = glGetUniformLocation(postProcessingProgramID, "effects");
		glUniform1f(currentTestLocation, effectCase);

		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;
		

		for (GameObject * pObj : carsList)
		{
			// updates car speed
			pObj->getObjectPhysics().setObjectSpeed(pObj->getObjectPhysics().getSpeed(), -1.f, 0.f);

			// check if car has gone past player
			if (pObj->getObjectTransform().getPosition().x - motorcycleObject->getObjectTransform().getPosition().x > 100)
			{
				// move car to be infront of player
				pObj->getObjectPhysics().setTransform(motorcycleObject->getObjectTransform().getPosition().x - 300, pObj->getObjectTransform().getPosition().y, pObj->getObjectTransform().getPosition().z);
				// change colour of car
				pObj->getMaterial().setSpecularMaterialColour(glm::vec4(((double)rand() / (RAND_MAX + 1)), ((double)rand() / (RAND_MAX + 1)), ((double)rand() / (RAND_MAX + 1)), 1.f));
				// give car a random speed
				pObj->getObjectPhysics().randomiseSpeed();
			}
		}


		for (GameObject * pObj : gameObjectsList)
		{
			// update location of physics objects
			if (pObj->getHasPhysics())
			{
				pObj->getObjectPhysics().update(dynamicsWorld);
				pObj->getObjectTransform().setPosition(pObj->getObjectPhysics().getObjectOrigin().getX(), pObj->getObjectPhysics().getObjectOrigin().getY(), pObj->getObjectPhysics().getObjectOrigin().getZ());
			}
			else
			{
				// move road to be infront of player
				if (pObj->getObjectTransform().getPosition().x - motorcycleObject->getObjectTransform().getPosition().x > 200)
				{
					pObj->getObjectTransform().setPosition(pObj->getObjectTransform().getPosition().x - 400, pObj->getObjectTransform().getPosition().y, pObj->getObjectTransform().getPosition().z);
				}
			}
		}

		dynamicsWorld->stepSimulation(1.f / 60.f, 10);

		for (GameObject * pObj : gameObjectsList)
		{
			pObj->getObjectTransform().update();

		}
		glEnable(GL_BLEND);  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		//Update Game and Draw with OpenGL!
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GameObject * pObj : gameObjectsList)
		{
			pObj->getMaterial().preRender();
			GLint currentShaderProgramID = pObj->getMaterial().getShaderProgramID();

			GLint viewMatrixLocation = glGetUniformLocation(currentShaderProgramID, "viewMatrix");
			GLint projectionMatrixLocation = glGetUniformLocation(currentShaderProgramID, "projectionMatrix");
			GLint camerPositionLocation = glGetUniformLocation(currentShaderProgramID, "cameraPosition");

			GLint lightDirectionLocation = glGetUniformLocation(currentShaderProgramID, "lightDirection");
			GLint ambientLightColourLocation = glGetUniformLocation(currentShaderProgramID, "ambientLightColour");
			GLint diffuseLightColourLocation = glGetUniformLocation(currentShaderProgramID, "diffuseLightColour");
			GLint specularLightColourLocation = glGetUniformLocation(currentShaderProgramID, "specularLightColour");

			GLint modelMatrixLocation = glGetUniformLocation(currentShaderProgramID, "modelMatrix");
			
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(pObj->getObjectTransform().getModelMatrix()));

			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(firstPersonCamera->getViewMatrix()));
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(firstPersonCamera->getProjectionMatrix()));
			glUniform3fv(camerPositionLocation, 1, value_ptr(firstPersonCamera->getCameraPosition()));

			glUniform3fv(lightDirectionLocation, 1, value_ptr(lightDirection));
			glUniform4fv(ambientLightColourLocation, 1, value_ptr(ambientLightColour));
			glUniform4fv(diffuseLightColourLocation, 1, value_ptr(diffuseLightColour));
			glUniform4fv(specularLightColourLocation, 1, value_ptr(specularLightColour));

			pObj->getMaterial().render();
		}


		
		glDisable(GL_BLEND);
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

	// detele physics objects
	groundShape->destroy(dynamicsWorld);

	for (GameObject * pObj : gameObjectsList)
	{
		if (pObj->getHasPhysics())
		{
			pObj->getObjectPhysics().destroy(dynamicsWorld);
		}
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	auto GameObjectIter = gameObjectsList.begin();
	while (GameObjectIter != gameObjectsList.end())
	{
		if ((*GameObjectIter))
		{
			(*GameObjectIter)->destroy();
			delete (*GameObjectIter);
			GameObjectIter = gameObjectsList.erase(GameObjectIter);
		}
	}

	glDeleteProgram(postProcessingProgramID);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenQuadVBOID);
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteRenderbuffers(1, &depthRenderBufferID);
	glDeleteTextures(1, &colourBufferID);


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
