#define GLEW_STATIC

#include "main.h"
#include <SFML/Window.hpp>
#include "Systems/InputManager.h"
#include "GameStates/GameState.h"
#include "GameStates/GameLevel.h"
#include "GameStates/Menu.h"

#include <sstream>
#include <fstream>
#include <direct.h>
#include "stdlib.h"
#include "time.h"
#include "Util/init.h"
#include "Util/text.h"
#include "Util/Mat4.h"
#include "Systems/Graphics/Shaders/shaders.h"

sf::Window sfmlWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "FlockingWars");

bool fullscreen = false;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
int uiWidth = 1024, uiHeight = 600;
const int SCREEN_BPP = 24;
const int FPS = 30;
bool quit = false;
bool DEBUG = false;
bool joystick = false;
bool mainMenuBuilt = false;
const int MAINMENU = 0, GAMEMODE = 1, STOREMODE = 0;
int gameMode = 0;
int mousex = 0, mousey = 0;
bool keys[323] = {false};


int volume = 0;

GameState* currentState = NULL;

InputManager inputManager;

GLuint bullet_mesh;
GLuint particle_DL;
GLuint glowTex;
GLuint buttonTex;
GLuint shipMesh;
GLuint attackBoidMesh;
GLuint boidSquadronDL;
GLuint logoTex;
GLuint logoDL;

void inputCallback (MappedInput& inputs);

using namespace std;

/**
 * init() initalizes SDL, creating a fullscreen window, and initalizes all the game objects
 * @return
 * 		bool flag stating whether the init succeeded with errors or not.
 */
bool init() {
	sfmlWindow.SetActive();
	//Initialize OpenGL
	if (init_GL(SCREEN_WIDTH, SCREEN_HEIGHT) == false) {
		return false;
	}
	srand(time(NULL));//Initialize random generator

	// Load Shaders
	Shader::initShaders();

	// Input Setup
	inputManager.registerCallback(inputCallback, 0);
	inputManager.pushContext("gameplay");
	return true;
}
/**
 * clean_up() releases all the memory that isn't automatically released and shuts SDL down.
 */
void clean_up() {
}


void inputCallback (MappedInput& inputs){
	if(inputs.actions.find(Input::A_QUIT_GAME) != inputs.actions.end()){
		quit = true;
	}
	if(currentState != NULL){
		currentState->handleInput(inputs, mousex, mousey, false);
	}
}

void pollEvents(){
	sf::Event event;
	while (sfmlWindow.GetEvent(event)) {
		switch(event.Type){
		case sf::Event::Closed:
			quit = true;
		break;
		case sf::Event::MouseMoved:
			mousex = event.MouseMove.X;
			mousey = SCREEN_HEIGHT - event.MouseMove.Y;
			break;
		case sf::Event::KeyPressed:
			keys[event.Key.Code] = true;
			cout << event.Key.Code << endl;
			break;
		case sf::Event::KeyReleased:
			inputManager.setRawInputState(event.Key.Code, false); // Tell inputManager that the key was released.
			keys[event.Key.Code] = false;
			break;
//		case SDL_MOUSEBUTTONDOWN:
//			keys[0] = true;
//			keys[1] = false;
//			break;
//		case SDL_MOUSEBUTTONUP:
//			keys[0] = false;
//			keys[1] = true;
//			break;
		}
	}
	// Register input for all keys currently down.
	for(int i = 0; i < 323; i++){
		if(keys[i]){
			inputManager.setRawInputState(i, true);
		}
	}
}

/**
 * Main method. Calls all the init functions, loads events from the event queue, has the main gameloops,
 * calls the update function and renders all the objects.
 */
int main() {

	if (init() == false) {
		return 1;
	}
	cout << "Init: complete" << endl;

	sf::Clock clock;
	currentState = new Menu(SCREEN_WIDTH, SCREEN_HEIGHT);
	currentState->init();
	float deltatime = 1;
	cout << "Entering main loop" << endl;
	while (quit == false) {
		pollEvents();
		inputManager.dispatchInput();
		inputManager.clear();
		clock.Reset();
		GameState* newState = currentState->update(deltatime);
		if(newState != currentState){
			delete currentState;
			currentState = newState;
			currentState->init();
		}
		currentState->render();
		deltatime = clock.GetElapsedTime();

		//Sleep the time remaining for a constant framerate to be maintained
		sfmlWindow.Display();
//		if ((1000 / FPS) > dif){
////			SDL_Delay((1000 / FPS) - dif);
//			deltaTime = (1000 / FPS);
//		}
//		else
//			deltaTime = dif;
	}
	//Free memory and quit SDL
	clean_up();
//	ctt.close();
	cout << "All done here." << endl;
	return 0;
}
