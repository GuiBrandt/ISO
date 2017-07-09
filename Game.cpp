#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Objeto do jogador
	/// </summary>
	Player Game::_player;

	/// <summary>
	/// Est�gio atual do jogo
	/// </summary>
	Stage Game::_currentStage("test");

	/// <summary>
	/// Evento em execu��o
	/// </summary>
	Event* Game::_currentEvent = nullptr;

	/// <summary>
	/// Inicializa��o do m�dulo do jogo
	/// </summary>
	void Game::initialize()
	{
		// TODO
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
	/// Obt�m o est�gio atual do jogo
	/// </summary>
	/// <returns></returns>
	Stage* Game::getCurrentStage()
	{
		return &_currentStage;
	}

	/// <summary>
	/// Muda o est�gio atual do jogo
	/// </summary>
	/// <param name="name">Nome do est�gio</param>
	void Game::changeStage(const char* name)
	{
		_currentStage = Stage(name);
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