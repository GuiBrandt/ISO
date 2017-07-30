#include <iso.hpp>
#include <string>

namespace Iso
{
	/// <summary>
	/// Objeto do jogador
	/// </summary>
	Player Game::_player;

	/// <summary>
	/// Est�gio atual do jogo
	/// </summary>
	Stage* Game::_currentStage;

	/// <summary>
	/// Mundo do jogo
	/// </summary>
	World* Game::_world;

	/// <summary>
	/// Evento em execu��o
	/// </summary>
	Event* Game::_currentEvent = nullptr;

	/// <summary>
	/// Janela do jogo
	/// </summary>
	GameWindow* Game::_window;

	/// <summary>
	/// Inicializa��o do m�dulo do jogo
	/// </summary>
	void Game::initialize()
	{
		// Objetos do jogo
		_window = new GameWindow();
		_world = new World();

		changeStage(getFirstStage());
	}

	/// <summary>
	/// Inicializa��o do m�dulo do jogo
	/// </summary>
	void Game::terminate()
	{
		delete _window;
		delete _currentStage;
	}


	/// <summary>
	/// Obt�m o objeto do jogador
	/// </summary>
	/// <returns>Um ponteiro para o objeto do jogador</returns>
	Player* Game::getPlayer()
	{
		return &_player;
	}

	/// <summary>
	/// Obt�m a janela do jogo
	/// </summary>
	/// <returns>A janela do jogo</returns>
	GameWindow* Game::getWindow()
	{
		return _window;
	}

	/// <summary>
	/// Obt�m o est�gio atual do jogo
	/// </summary>
	/// <returns>O est�gio atual do jogo</returns>
	Stage* Game::getCurrentStage()
	{
		return _currentStage;
	}

	/// <summary>
	/// Obt�m o mundo do jogo
	/// </summary>
	/// <returns>Inst�ncia do mundo do jogo</returns>
	World* Game::getWorld()
	{
		return _world;
	}

	/// <summary>
	/// Muda o est�gio atual do jogo
	/// </summary>
	/// <param name="stage">Est�gio</param>
	void Game::changeStage(Stage* stage)
	{
		if (_currentStage)
		{
			_world->clear();
			delete _currentStage;
		}

		_currentStage = stage;
		_currentStage->setup();
	}

	/// <summary>
	/// Obt�m um ponteiro para o evento em execu��o
	/// </summary>
	/// <returns>Um ponteiro para o evento sendo executado</returns>
	Event* Game::getCurrentEvent()
	{
		return _currentEvent;
	}

	/// <summary>
	/// Define o evento em execu��o pelo jogo
	/// </summary>
	/// <param name="ev">Ponteiro para o evento</param>
	void Game::setCurrentEvent(Event* ev)
	{
		_currentEvent = ev;
	}
};