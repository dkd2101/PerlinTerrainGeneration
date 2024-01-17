#include "SDLGraphicsProgram.hpp"


// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our Renderer
    m_renderer = new Renderer(w,h);
    myTerrain = nullptr;
    fractal = false;
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }

    if(terrainNode != nullptr){
        delete terrainNode;
    }

    if(myTerrain != nullptr){
        delete myTerrain;
    }

    if(bushQuad != nullptr){
        delete bushQuad;
    }

    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

	return success;
}

void SDLGraphicsProgram::generateRandomMap(){
	Perlin newMap = Perlin();

	newMap.generatePPM(this->fractal);
	newMap.savePPM("", "perlin_texture.ppm");
}

void SDLGraphicsProgram::terrainHelper(){

    std::cout << "loading new terrain" << std::endl;

    std::cout << "generating new terrain" << std::endl;
    if(myTerrain != nullptr){
        delete myTerrain;
        myTerrain = nullptr;
    } 

    myTerrain = new Terrain(512,512,"./perlin_texture.ppm");
    myTerrain->LoadTexture("./../common/textures/Stone.ppm"); 
    myTerrain->LoadGradientTextures("./../common/textures/Grass.ppm", "./../common/textures/Dirt.ppm", 
                                                "./../common/textures/Stone.ppm", "./../common/textures/Snow.ppm");
    
    std::cout << "Creating new node" << std::endl;

    terrainNode = new SceneNode(myTerrain);

    m_renderer->setRoot(terrainNode);

    std::cout << "Creating new object" << std::endl;
    bushQuad = new Object();
    bushQuad->LoadTexture("./../common/textures/bush.ppm"); 
    bushQuad->MakeTexturedQuad("./../common/textures/bush.ppm");

    std::cout << "iterating through nodes" << std::endl;
    for(int i = 0; i < myTerrain->m_treePositions.size() / 3; i++){
        terrainNode->AddChild(new SceneNode(bushQuad));
    }
    std::cout << "finished creating trees" << std::endl;
}

//Loops forever!
void SDLGraphicsProgram::Loop(){
    // Create our terrain

    terrainHelper();

    // Set a default position for our camera
    m_renderer->GetCamera(0)->SetCameraEyePosition(125.0f,50.0f,500.0f);

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 5.0f;

    // While application is running
    while(!quit){

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboard input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
            }

            // had to change the way this specific key was handled because a key press
		    // gets called twice each time.
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_n && e.key.repeat == 0){
                    generateRandomMap();
                    terrainHelper();
                }
                if(e.key.keysym.sym == SDLK_f && e.key.repeat == 0){
                    fractal = !fractal;
                }
            }


            const Uint8 *state = SDL_GetKeyboardState(NULL);

            // Camera
            // Update our position of the camera
            if (state[SDL_SCANCODE_W]) {
                m_renderer->GetCamera(0)->ProcessKeyboard(FORWARD);
            }
            if (state[SDL_SCANCODE_S]) {
                m_renderer->GetCamera(0)->ProcessKeyboard(BACKWARD);
            }
            if (state[SDL_SCANCODE_A]) {
                m_renderer->GetCamera(0)->ProcessKeyboard(LEFT);
            }
            if (state[SDL_SCANCODE_D]) {
                m_renderer->GetCamera(0)->ProcessKeyboard(RIGHT);
            }
            if (state[SDL_SCANCODE_SPACE]) {
                m_renderer->GetCamera(0)->ProcessKeyboard(UP);
            }
            if (state[SDL_SCANCODE_RSHIFT]) {
                m_renderer->GetCamera(0)->ProcessKeyboard(DOWN);
            }
            if (state[SDL_SCANCODE_Q]) {
                quit = true;
            }


        } // End SDL_PollEvent loop.

        terrainNode->GetLocalTransform().LoadIdentity();

        // Since we loaded the bushes into the child vector of our terrain we can now apply our list of generated locations to place them
        terrainNode->TranslateChildren(myTerrain->m_treePositions);
        
        // Update our scene through our renderer
        m_renderer->Update();
        // Render our scene using our selected renderer
        m_renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}




// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
