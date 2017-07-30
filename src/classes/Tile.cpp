#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="pos">Posição do tile</param>
	/// <param name="color">Cor do tile</param>
	/// <param name="type">Tipo do tile</param>
	Tile::Tile(point3f pos, RGB color, TileType type) : GameObject(
		pos,
		color,
		type == Wall ? Models::WALL : Models::FLOOR, 
		type == Wall ? 24 : 8
	)
	{
		_type = type;
	}

	/// <summary>
	/// Verifica se o tile é passável
	/// </summary>
	/// <returns>Verdadeiro se for chão e falso se for parede</returns>
	bool Tile::isPassable() 
	{
		return _type == Floor;
	}

	/// <summary>
	/// Verifica se o objeto é um tile
	/// </summary>
	/// <returns>Verdadeiro</returns>
	bool Tile::isTile()
	{
		return true;
	}
};