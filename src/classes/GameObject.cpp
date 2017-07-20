#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Obt�m a posi��o do objeto no mundo
	/// </summary>
	/// <returns>point3f para a posi��o do objeto</returns>
	point3f GameObject::getPosition()
	{
		return { _x, _y, _z };
	}

	/// <summary>
	/// Move o objeto
	/// </summary>
	/// <param name="x">Deslocamento no eixo X</param>
	/// <param name="y">Deslocamento no eixo Y</param>
	/// <param name="z">Deslocamento no eixo Z</param>
	void GameObject::move(GLfloat x, GLfloat y, GLfloat z)
	{
		_x += x;
		_y += y;
		_z += z;
	}

	/// <summary>
	/// Move o objeto para uma posi��o absoluta
	/// </summary>
	/// <param name="x">Posi��o X</param>
	/// <param name="y">Posi��o Y</param>
	/// <param name="z">Posi��o Z</param>
	void GameObject::moveTo(GLfloat x, GLfloat y, GLfloat z)
	{
		_x = x;
		_y = y;
		_z = z;
	}
};