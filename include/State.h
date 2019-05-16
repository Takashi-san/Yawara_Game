#include "GameObject.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <vector>

#ifndef STATE
	#define STATE

	class State {
	protected:
		bool popRequested;
		bool quitRequested;
		bool started;

		std::vector<std::shared_ptr<GameObject>> objectArray;

		virtual void StartArray();
		virtual void UpdateArray(float dt);
		virtual void RenderArray();
		
	public:
		State();
		virtual ~State();

		virtual void LoadAssets() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

		virtual void Start() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;

		virtual std::weak_ptr<GameObject> AddObject(GameObject*);
		virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject*);

		bool PopRequested();
		bool QuitRequested();
	};

#endif