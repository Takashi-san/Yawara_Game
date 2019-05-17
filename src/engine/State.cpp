#include "State.h"

State::State() {
	popRequested = false;
	quitRequested = false;
	started = false;
}

State::~State() {
	objectArray.clear();
}

std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
	std::shared_ptr<GameObject> s_ptr(go);
	std::weak_ptr<GameObject> weak;

	objectArray.emplace_back(s_ptr);
	if (started) {
		objectArray.back()->Start();
	}

	weak = objectArray.back();
	return weak;
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
	std::weak_ptr<GameObject> weak;

	for (unsigned int i = 0; i < objectArray.size(); i++) {
		if (objectArray[i].get() == go) {
			weak = objectArray[i];
			return weak;
		}
	}

	return weak;
}

bool State::PopRequested() {
	return popRequested;
}

bool State::QuitRequested() {
	return quitRequested;
}

void State::StartArray(){
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Start();
	}
}

void State::UpdateArray(float dt) {
	// update dos game object.
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Update(dt);
	}
	
	// deleta objetos que devem sumir.
	for (std::vector<std::shared_ptr<GameObject>>::iterator i = objectArray.begin(); i < objectArray.end(); i++) {
		if (i->get()->IsDead()) {
			i = objectArray.erase(i);
			i--;
		}
	}
}

void State::RenderArray() {
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		objectArray[i]->Render();
	}
}