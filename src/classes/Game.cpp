#include <iso.hpp>

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
	/// Muda o título da janela do jogo
	/// </summary>
	shiro_native(setTitle)
	{
		printf("asd");
		shiro_push_arg_c(title, string, 0);

		Game::getWindow()->setTitle(title);

		return shiro_nil;
	}

	/// <summary>
	/// Inicialização do módulo do jogo
	/// </summary>
	void Game::initialize()
	{
		// Inicialização do Shiro
		_scriptRuntime = shiro_init();

		shiro_def_native(_scriptRuntime, setTitle, 1);

		_window = new GameWindow();
		_currentStage = new Stage("test");
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
		delete _currentStage;
		_currentStage = new Stage(name);
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
		char* c = (char*)malloc(strlen(code) + 1);
		memcpy_s(c, strlen(code) + 1, code, strlen(code));

		shiro_binary* bin = shiro_compile(c);
		shiro_execute(_scriptRuntime, bin);
	}
};