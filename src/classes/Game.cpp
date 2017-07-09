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
	Stage Game::_currentStage("test");

	/// <summary>
	/// Evento em execução
	/// </summary>
	Event* Game::_currentEvent = nullptr;

	/// <summary>
	/// Inicialização do módulo do jogo
	/// </summary>
	void Game::initialize()
	{
		// TODO
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
	/// Obtém o estágio atual do jogo
	/// </summary>
	/// <returns></returns>
	Stage* Game::getCurrentStage()
	{
		return &_currentStage;
	}

	/// <summary>
	/// Muda o estágio atual do jogo
	/// </summary>
	/// <param name="name">Nome do estágio</param>
	void Game::changeStage(const char* name)
	{
		_currentStage = Stage(name);
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
};