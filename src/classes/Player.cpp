#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	Player::Player() : GameObject(point3f{ 0, 0, 0 }, RGB { 0, 0, 0 }, Models::CHARACTER, 24) {}

	/// <summary>
	/// Verifica se o objeto é um jogador
	/// </summary>
	/// <returns>Verdadeiro</returns>
	bool Player::isPlayer() 
	{
		return true;
	}
}