#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Modelo do jogador
	/// </summary>
	const float Models::PLAYER[] = {
		// Face da esquerda
		0, 1, 0,	-1, 0, 0,
		0, 1, -1,	-1, 0, 0,
		0, 0, -1,	-1, 0, 0,
		0, 0, 0,	-1, 0, 0,
		 
		// Face da frente
		1, 1, 0,	0, 1, 0,
		1, 1, -1,	0, 1, 0,
		0, 1, -1,	0, 1, 0,
		0, 1, 0,	0, 1, 0,		

		// Face de baixo
		0, 1, 0,	0, 0, 1,
		1, 1, 0,	0, 0, 1,
		1, 0, 0,	0, 0, 1,
		0, 0, 0,	0, 0, 1,

		// Face de trás
		0, 0, 0,	0, -1, 0,
		1, 0, 0,	0, -1, 0,
		1, 0, -1,	0, -1, 0,
		0, 0, -1,	0, -1, 0,

		// Face da direita
		1, 0, 0,	1, 0, 0,
		1, 1, 0,	1, 0, 0,
		1, 1, -1,	1, 0, 0,
		1, 0, -1,	1, 0, 0,

		// Face de cima
		1, 1, -1,	0, 0, -1,
		0, 1, -1,	0, 0, -1,
		0, 0, -1,	0, 0, -1,
		1, 0, -1,	0, 0, -1,
	};

	/// <summary>
	/// Modelo do chão
	/// </summary>
	const float Models::FLOOR[] = {
		0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0
	};

	/// <summary>
	/// Modelo da parede
	/// </summary>
	const float Models::WALL[] = {
		// Face de baixo
		0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,

		// Face da esquerda
		0, 0, 0,
		0, 0, 1,
		0, 1, 1,
		0, 1, 0,

		// Face da direita
		1, 0, 0,
		1, 0, 1,
		1, 1, 1,
		1, 1, 0,

		// Face da frente
		0, 1, 0,
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,

		// Face de trás
		0, 1, 0,
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,

		// Face de cima
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1,
	};
};