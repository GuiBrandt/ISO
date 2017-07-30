#include <iso.hpp>
#include <thread>
#include <chrono>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="pos">Posição do objeto</param>
	/// <param name="color">Cor do objeto</param>
	/// <param name="model">Modelo do objeto</param>
	/// <param name="vertexCount">Número de vértices do objeto</param>
	GameObject::GameObject(point3f pos, RGB color, const GLfloat* model, int vertexCount)
	{
		_position = pos;
		_desiredPosition = pos;
		_color = color;
		_model = model;
		_vertexCount = vertexCount;
		_visible = true;
	}

	/// <summary>
	/// Obtém a posição do objeto no mundo
	/// </summary>
	/// <returns>point3f para a posição do objeto</returns>
	point3f GameObject::getPosition()
	{
		return _position;
	}

	/// <summary>
	/// Define a posição do objeto no mundo
	/// </summary>
	/// <param name="pos">Posição</param>
	void GameObject::setPosition(point3f pos)
	{
		_position = pos;
		_desiredPosition = _position;
	}

	/// <summary>
	/// Move o objeto
	/// </summary>
	/// <param name="x">Deslocamento no eixo X</param>
	/// <param name="y">Deslocamento no eixo Y</param>
	void GameObject::move(int x, int y)
	{
		World* world = Game::getWorld();
		GameObject* obj;

		if (_desiredPosition.x != _position.x || _desiredPosition.y != _position.y || _desiredPosition.z != _position.z)
			return;

		int px = (int)round(_position.x), py = (int)round(_position.y), pz = (int)round(_position.z);

		int z = 0;

		if (!world->isPassable(px + x, py + y, pz, &obj))
		{
			if (world->isPassable(px + x, py + y, pz - 1) && world->isEmpty(px, py, pz - 1))
				z = -1;
			else
			{
				onCollided(obj);
				obj->onHit(this);

				return;
			}
		}

		_desiredPosition = { (GLfloat)px + x, (GLfloat)py + y, (GLfloat)pz + z };
	}

	/// <summary>
	/// Move o objeto para uma posição absoluta
	/// </summary>
	/// <param name="x">Posição X</param>
	/// <param name="y">Posição Y</param>
	void GameObject::moveTo(int x, int y)
	{
		move(x - (int)_position.x, y - (int)_position.y);
	}

	/// <summary>
	/// Obtém o modelo 3D de um objeto
	/// </summary>
	/// <param name="vertexCount">Número de vértices do modelo</param>
	/// <returns>Um array de floats para o modelo</returns>
	const GLfloat* GameObject::getModel(int& vertexCount)
	{
		vertexCount = _vertexCount;
		return _model;
	}

	/// <summary>
	/// Obtém a cor do objeto
	/// </summary>
	/// <returns>RGB do objeto</returns>
	RGB GameObject::getColor()
	{
		return _color;
	}

	/// <summary>
	/// Define a cor do objeto
	/// </summary>
	void GameObject::setColor(RGB rgb)
	{
		_color = rgb;
	}

	/// <summary>
	/// Mostra o objeto
	/// </summary>
	void GameObject::show()
	{
		_visible = true;
	}

	/// <summary>
	/// Esconde o objeto
	/// </summary>
	void GameObject::hide()
	{
		_visible = false;
	}

	/// <summary>
	/// Verifica se o objeto está visível
	/// </summary>
	/// <returns>Verdadeiro se sim e falso se não</returns>
	bool GameObject::isVisible()
	{
		return _visible;
	}

	/// <summary>
	/// Atualiza o objeto
	/// </summary>
	void GameObject::update()
	{
		World* world = Game::getWorld();
		GameObject* obj;

		if (!world->isPassable((int)_desiredPosition.x, (int)_desiredPosition.y, (int)_desiredPosition.z, &obj) && obj != this)
			_desiredPosition = _position;

		if (_desiredPosition.x != _position.x || _desiredPosition.y != _position.y || _desiredPosition.z != _position.z)
		{
			if (_desiredPosition.z != _position.z)
				_position.z += copysign(0.125f, _desiredPosition.z - _position.z);
			else
			{
				if (_desiredPosition.x != _position.x)
					_position.x += copysign(0.125f, _desiredPosition.x - _position.x);

				if (_desiredPosition.y != _position.y)
					_position.y += copysign(0.125f, _desiredPosition.y - _position.y);
			}
		}
		else if (!isTile() && world->isEmpty((int)_position.x, (int)_position.y, (int)_position.z) && (world->isPassable((int)_position.x, (int)_position.y, (int)_position.z + 1, &obj) || obj == this))
			_desiredPosition.z = _position.z + 1;			
	}
};