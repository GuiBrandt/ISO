#include <iso.hpp>

using namespace std;

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="pos">Posição</param>
	/// <param name="color">Cor</param>
	Event::Event(point3f pos, RGB color) : GameObject(pos, color, Models::CHARACTER, 24) {}

	/// <summary>
	/// Verifica se o objeto é um evento
	/// </summary>
	/// <returns>Verdadeiro</returns>
	bool Event::isEvent() 
	{
		return true;
	}
};