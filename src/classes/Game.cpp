#include <iso.hpp>
#include <string>

namespace Iso
{
	/// <summary>
	/// Objeto do jogador
	/// </summary>
	Player Game::_player;

	/// <summary>
	/// Estágio atual do jogo
	/// </summary>
	Stage* Game::_currentStage;

	/// <summary>
	/// Evento em execução
	/// </summary>
	Event* Game::_currentEvent = nullptr;

	/// <summary>
	/// Janela do jogo
	/// </summary>
	GameWindow* Game::_window;

	/// <summary>
	/// Runtime do Shiro
	/// </summary>
	shiro_runtime* Game::_scriptRuntime;

	/// <summary>
	/// Inicialização do módulo do jogo
	/// </summary>
	void Game::initialize()
	{
		// Inicialização do Shiro
		_scriptRuntime = shiro_init();

		shiro_def_native(_scriptRuntime, setTitle, 1);
		shiro_def_native(_scriptRuntime, setPass, 2);
		shiro_def_native(_scriptRuntime, changeStage, 1);
		shiro_def_native(_scriptRuntime, show, 0);
		shiro_def_native(_scriptRuntime, hide, 0);
		shiro_def_native(_scriptRuntime, eventIsVisible, 0);

		// Objetos do jogo
		_window = new GameWindow();

		changeStage("test");

		_player.move(2, 2, 0);
	}

	/// <summary>
	/// Inicialização do módulo do jogo
	/// </summary>
	void Game::terminate()
	{
		delete _window;
		delete _currentStage;
	}


	/// <summary>
	/// Obtém o objeto do jogador
	/// </summary>
	/// <returns>Um ponteiro para o objeto do jogador</returns>
	Player* Game::getPlayer()
	{
		return &_player;
	}

	/// <summary>
	/// Obtém a janela do jogo
	/// </summary>
	/// <returns>A janela do jogo</returns>
	GameWindow* Game::getWindow()
	{
		return _window;
	}

	/// <summary>
	/// Obtém o estágio atual do jogo
	/// </summary>
	/// <returns></returns>
	Stage* Game::getCurrentStage()
	{
		return _currentStage;
	}

	/// <summary>
	/// Muda o estágio atual do jogo
	/// </summary>
	/// <param name="name">Nome do estágio</param>
	void Game::changeStage(const char* name)
	{
		if (_currentStage)
			delete _currentStage;

		_currentStage = new Stage(name);

		std::string ename = name;
		ename += "-init";

		Event(ename.c_str(), RGB{ 0, 0, 0 }).start();
	}

	/// <summary>
	/// Obtém um ponteiro para o evento em execução
	/// </summary>
	/// <returns>Um ponteiro para o evento sendo executado</returns>
	Event* Game::getCurrentEvent()
	{
		return _currentEvent;
	}

	/// <summary>
	/// Define o evento em execução pelo jogo
	/// </summary>
	/// <param name="ev">Ponteiro para o evento</param>
	void Game::setCurrentEvent(Event* ev)
	{
		_currentEvent = ev;
	}

	/// <summary>
	/// Executa um script
	/// </summary>
	/// <param name="code">Código que será executado</param>
	void Game::runScript(const char* code)
	{
		char* c = (char*)calloc(strlen(code) + 1, sizeof(char));
		memcpy_s(c, strlen(code) + 1, code, strlen(code));

		shiro_binary* bin = shiro_compile(c);

		if (bin != NULL)
			shiro_execute(_scriptRuntime, bin);
		else
			fprintf(stderr, "%s\n", shiro_get_last_error());
	}
};