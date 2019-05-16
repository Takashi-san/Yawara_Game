#include "Game.h"
#include "Resources.h"
#include "InputManager.h"

Game* Game::instance;

Game::Game(std::string title, int width, int height) {
	int i;

	dt = 0;
	frameStart = SDL_GetTicks();
	storedState = nullptr;

	// Seed do random.
	srand(time(NULL));

	// Verifica se ja existe um objeto da classe.
	if (instance != nullptr){
		// Erro na lógica do programa.
		std::cout << "Erro lógica. Tentativa de criar multiplos game.\n";
		exit(EXIT_FAILURE);
	} else {
		instance = this;
	}

	// Inicia a biblioteca SDL definindo quais subsistemas vamos iniciar.
	i = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	if (i != 0) {
		// Caso de falha na inicializazao encerrar programa.
		std::cout << "Erro inicialização SDL: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	// Inicia a SDL_Image.
	i = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	if (i == 0) {
		// falhou em iniciar todos.
		std::cout << "Erro inicialização SDL_Image: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	// Inicia a SDL_mixer.
	i = Mix_Init(MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MP3 );
	if (i == 0) {
		// falhou em iniciar todos.
		std::cout << "Erro inicialização SDL_Mixer: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}
	i = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	if (i != 0) {
		// falhou na inicializacao.
		std::cout << "Erro inicialização Mix_OpenAudio: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}
	Mix_AllocateChannels(32);

	// Inicia a SDL_ttf.
	i = TTF_Init();
	if (i != 0) {
		// falhou na inicializacao.
		std::cout << "Erro inicialização TTF_Init: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	// Iniciar janela.
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (window == nullptr){
		// falhou na inicializacao.
		std::cout << "Erro inicialização SDL_CreateWindow: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);		
	}

	// Iniciar renderizador.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr){
		// falhou na inicializacao.
		std::cout << "Erro inicialização SDL_CreateRenderer: " << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}
}

Game::~Game() {
	// Esvazia pilha de estados e o estado armazenado.
	if (storedState != nullptr){
		delete storedState;
	}
	while (!stateStack.empty()) {
		stateStack.pop();
	}

	// Limpando os recursos.
	Resources::ClearImages();
	Resources::ClearSounds();
	Resources::ClearMusics();
	Resources::ClearFonts();

	// Precisa desfazer na ordem inversa aa inicializacao.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

Game& Game::GetInstance() {
	// Verifica se ja existe um objeto da classe.
	if(instance == nullptr){
		// Se não existir, o cria.
		instance = new Game("BrunoTakashiTengan120167263", 1024, 600);
	}

	// Retorna objeto da classe existente.
	return *instance;
}

SDL_Renderer* Game::GetRenderer() {
	return renderer;
}

State& Game::GetCurrentState() {
	return *(stateStack.top());
}

void Game::Push(State* state) {
	storedState = state;
}

void Game::Run() {
	InputManager& input = InputManager::GetInstance();

	// Carrega estado inicial.
	if (storedState != nullptr) {
		stateStack.emplace(storedState);
		stateStack.top()->Start();
		storedState = nullptr;
	} else {
		return;
	}

	// Main game loop.
	while (!stateStack.top()->QuitRequested() && !stateStack.empty()) {
		// Gerenciando a pilha de estados.
		if (stateStack.top()->PopRequested()) {
			stateStack.pop();

			// Libera recursos não sendo usados.
			Resources::ClearImages();
			Resources::ClearSounds();
			Resources::ClearMusics();
			Resources::ClearFonts();
			
			if (!stateStack.empty()) {
				stateStack.top()->Resume();
			}
		}
		if (storedState != nullptr) {
			if (!stateStack.empty()) {
				stateStack.top()->Pause();
			}
			stateStack.emplace(storedState);
			stateStack.top()->Start();
			storedState = nullptr;
		} else if (stateStack.empty()) {
			break;
		}

		// Execução do estado atual.
		CalculateDeltaTime();
		input.Update();
		stateStack.top()->Update(dt);
		stateStack.top()->Render();
		SDL_RenderPresent(renderer);
		SDL_Delay(33);
	}

	// Limpando os recursos.
	while (!stateStack.empty()) {
		stateStack.pop();
	}
	Resources::ClearImages();
	Resources::ClearSounds();
	Resources::ClearMusics();
	Resources::ClearFonts();
}

void Game::CalculateDeltaTime() {
	dt = (float)(SDL_GetTicks() - frameStart)/1000;
	frameStart = SDL_GetTicks();
}

float Game::GetDeltaTime() {
	return dt;
}