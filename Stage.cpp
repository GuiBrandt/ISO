#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="name">Nome do estágio</param>
	Stage::Stage(const char* name)
	{
		// TODO
	}

	/// <summary>
	/// Destrutor
	/// </summary>
	Stage::~Stage()
	{
		delete [] _layers;
	}

	/// <summary>
	/// Renderiza a cena na tela
	/// </summary>
	void Stage::render(void)
	{
	}

	/// <summary>
	/// Verifica se uma posição é passável
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <returns></returns>
	bool Stage::isPassable(int x, int y, int z)
	{
		return false;
	}

	/// <summary>
	/// Obtém o evento em uma determinada posição
	/// </summary>
	/// <param name="x">X da posição</param>
	/// <param name="y">Y da posição</param>
	/// <param name="z">Z da posição</param>
	/// <returns>Um ponteiro para o evento ou nullptr caso ele não exista</returns>
	Event* Stage::eventAt(int x, int y, int z)
	{
		Event* ev = nullptr;

		for each (auto pair in _events)
			if (pair.first.x == x && pair.first.y == y && pair.first.z == z)
				ev = &pair.second;

		return ev;
	}
};