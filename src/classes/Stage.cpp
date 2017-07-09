#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="name">Nome do est�gio</param>
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
	/// Verifica se uma posi��o � pass�vel
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
	/// Obt�m o evento em uma determinada posi��o
	/// </summary>
	/// <param name="x">X da posi��o</param>
	/// <param name="y">Y da posi��o</param>
	/// <param name="z">Z da posi��o</param>
	/// <returns>Um ponteiro para o evento ou nullptr caso ele n�o exista</returns>
	Event* Stage::eventAt(int x, int y, int z)
	{
		Event* ev = nullptr;

		for each (auto pair in _events)
			if (pair.first.x == x && pair.first.y == y && pair.first.z == z)
				ev = &pair.second;

		return ev;
	}
};