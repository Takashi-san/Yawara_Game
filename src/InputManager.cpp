#include "InputManager.h"

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

	mouseX = 0;
	mouseY = 0;
}

InputManager::~InputManager() {
	keyState.clear();
	keyUpdate.clear();
}

void InputManager::Update() {
	SDL_Event event;

	// Obtem coordenadas do mouse.
	SDL_GetMouseState(&mouseX, &mouseY);

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
		}
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

bool InputManager::QuitRequested() {
	return quitRequested;
}