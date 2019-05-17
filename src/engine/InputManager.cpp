#include "InputManager.h"
#include "Game.h"
#include "Vec2.h"

InputManager& InputManager::GetInstance(){
	static InputManager instance;
	return instance;
}

InputManager::InputManager() {
	for (int i = 0; i < 6; i++) {
		mouseState[i] = 0;
		mouseUpdate[i] = 0;
	}

	quitRequested = false;
	updateCounter = 0;

	mouseX = Game::GetInstance().GetWindowSize().x/2;
	mouseY = Game::GetInstance().GetWindowSize().y/2;
	mouseS = 1;
}

InputManager::~InputManager() {
	keyState.clear();
	keyUpdate.clear();
}

void InputManager::Update() {
	SDL_Event event;

	// Reseta quitRequested.
	quitRequested = false;

	// Contador de updates.
	updateCounter++;

	// Retorna true enquanto tiver eventos para repassar.
	while (SDL_PollEvent(&event)) {
		// Verifica se o evento é repetido.
		if (!event.key.repeat) {

			// Alt+F4 ou fechar a janela pelo botão X, etc.
			if(event.type == SDL_QUIT) {
				quitRequested = true;
			}

			// Pressionar botão do mouse.
			if(event.type == SDL_MOUSEBUTTONDOWN) {
				mouseState[event.button.button] = true;
				mouseUpdate[event.button.button] = updateCounter;
			}
			// Soltar botão do mouse.
			if(event.type == SDL_MOUSEBUTTONUP) {
				mouseState[event.button.button] = false;
				mouseUpdate[event.button.button] = updateCounter;
			}


			// Pressionar teclado.
			if(event.type == SDL_KEYDOWN) {
				keyState[event.key.keysym.sym] = true;
				keyUpdate[event.key.keysym.sym] = updateCounter;
			}
			// Soltar teclado.
			if(event.type == SDL_KEYUP) {
				keyState[event.key.keysym.sym] = false;
				keyUpdate[event.key.keysym.sym] = updateCounter;
			}

			// Obtem coordenadas do mouse. Possibilidade de controlar sensitividade do mouse.
			if((event.type == SDL_MOUSEMOTION) && SDL_GetRelativeMouseMode()) {
				mouseX += (int)(mouseS * event.motion.xrel);
				mouseY += (int)(mouseS * event.motion.yrel);

				// Limita o mouse na janela.
				Vec2 window = Game::GetInstance().GetWindowSize();
				if (mouseX < 0) {
					mouseX = 0;
				} else if (mouseX > window.x) {
					mouseX = window.x;
				}
				if (mouseY < 0) {
					mouseY = 0;
				} else if (mouseY > window.y) {
					mouseY = window.y;
				}
			}
		}
	}

	// Obtem coordenadas do mouse. EASY AND RAW. Caso relative mode dê erro.
	if (!SDL_GetRelativeMouseMode()) {
		SDL_GetMouseState(&mouseX, &mouseY);
	}
}

bool InputManager::KeyPress(int key) {
	return keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::KeyRelease(int key) {
	return !keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::MousePress(int button) {
	return mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::MouseRelease(int button) {
	return !mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::IsKeyDown(int key) {
	return keyState[key];
}

bool InputManager::IsMouseDown(int button) {
	return mouseState[button];
}

int InputManager::GetMouseX() {
	return mouseX;
}

int InputManager::GetMouseY() {
	return mouseY;
}

void InputManager::SetMouseS(float mouseS) {
	this->mouseS = mouseS;
}

bool InputManager::QuitRequested() {
	return quitRequested;
}