#include <iso.hpp>

#include <algorithm>
#include <fstream>
#include <string>
#include <ctype.h>

using namespace std;

namespace Iso
{
	/// <summary>
	/// Adiciona um objeto ao mundo
	/// </summary>
	/// <param name="obj">Objeto</param>
	void World::add(GameObject* obj)
	{
		_objects.push_back(obj);
	}

	/// <summary>
	/// Remove um objeto do mundo
	/// </summary>
	/// <param name="obj">Objeto</param>
	void World::remove(GameObject* obj)
	{
		_objects.erase(std::remove(_objects.begin(), _objects.end(), obj), _objects.end());
	}

	/// <summary>
	/// Limpa a lista de objetos
	/// </summary>
	void World::clear()
	{
		_objects.clear();
	}

	/// <summary>
	/// Renderiza um modelo em uma posi��o
	/// </summary>
	/// <param name="model">Modelo a ser desenhado</param>
	/// <param name="vertexCount">N�mero de v�rtices do modelo</param>
	/// <param name="color">Cor do modelo</param>
	/// <param name="position">Posi��o</param>
	/// <param name="mode">Modo de renderiza��o</param>
	void renderModel(const GLfloat* model, int vertexCount, RGB color, point3f position, GLenum mode = GL_QUADS)
	{
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);

		glColor3fv((float*)&color);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), model);
		glNormalPointer(GL_FLOAT, 6 * sizeof(float), &model[3]);
		glDrawArrays(mode, 0, vertexCount);
		glPopMatrix();
	}

	/// <summary>
	/// Atualiza todos os objetos
	/// </summary>
	void World::update()
	{
		for each (GameObject* obj in _objects)
			obj->update();
	}

	/// <summary>
	/// Renderiza a cena na tela
	/// </summary>
	void World::render()
	{
		glClearColor(_bgColor.red, _bgColor.green, _bgColor.blue, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// Ilumina��o
		//
		
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glLoadIdentity();

		glLightfv(GL_LIGHT0, GL_AMBIENT, (float*)&_ambientLightColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, (float*)&_diffuseLightColor);
		glLightfv(GL_LIGHT0, GL_POSITION, (float*)&_lightOrigin);

		glPopMatrix();

		//
		// Objetos
		//

		glPushMatrix();

		for each (GameObject* obj in _objects)
		{
			if (!obj->isVisible())
				continue;

			int vxc = 0;
			const GLfloat* model = obj->getModel(vxc);
			renderModel(model, vxc, obj->getColor(), obj->getPosition());
		}

		glPopMatrix();
	}

	/// <summary>
	/// Verifica se uma posi��o � pass�vel
	/// </summary>
	/// <param name="x">X da posi��o</param>
	/// <param name="y">Y da posi��o</param>
	/// <param name="z">Z da posi��o</param>
	/// <param name="pobj">Ponteiro para ponteiro de objeto, serve para retornar o objeto que torna a posi��o impass�vel</param>
	/// <returns>Verdadeiro se for e falso se n�o</returns>
	bool World::isPassable(int x, int y, int z, GameObject** pobj)
	{
		for each (GameObject* obj in _objects)
		{
			int ox, oy, oz;
			point3f pos = obj->getPosition();
			ox = (int)round(pos.x);
			oy = (int)round(pos.y);
			oz = (int)round(pos.z);

			if (!obj->isPassable() && ox == x && oy == y && oz == z)
			{
				if (pobj)
					*pobj = obj;

				return false;
			}
		}

		if (pobj)
			*pobj = NULL;

		return true;
	}

	/// <summary>
	/// Verifica se uma posi��o est� sem tiles
	/// </summary>
	/// <param name="x">X da posi��o</param>
	/// <param name="y">Y da posi��o</param>
	/// <param name="z">Z da posi��o</param>
	/// <param name="pobj">Ponteiro para ponteiro de objeto, serve para retornar o objeto que torna a posi��o impass�vel</param>
	/// <returns>Verdadeiro se estiver e falso se n�o</returns>
	bool World::isEmpty(int x, int y, int z, GameObject** pobj)
	{
		for each (GameObject* obj in _objects)
		{
			int ox, oy, oz;
			point3f pos = obj->getPosition();
			ox = (int)round(pos.x);
			oy = (int)round(pos.y);
			oz = (int)-round(pos.z);

			if (ox == x && oy == y && oz == z)
			{
				if (pobj)
					*pobj = obj;
				return false;
			}
		}
		
		if (pobj)
			*pobj = NULL;

		return true;
	}

	/// <summary>
	/// Obt�m o evento em uma determinada posi��o
	/// </summary>
	/// <param name="x">X da posi��o</param>
	/// <param name="y">Y da posi��o</param>
	/// <param name="z">Z da posi��o</param>
	/// <returns>Um ponteiro para o evento ou nullptr caso ele n�o exista</returns>
	Event* World::eventAt(int x, int y, int z)
	{
		Event* ev = nullptr;

		for each (GameObject* obj in _objects)
		{
			int ox, oy, oz;
			point3f pos = obj->getPosition();
			ox = (int)round(pos.x);
			oy = (int)round(pos.y);
			oz = (int)-round(pos.z);

			if (obj->isEvent() && ox == x && oy == y && oz == z)
				ev = (Event*)obj;
		}

		return ev;
	}

	/// <summary>
	/// Obt�m a cor do fundo
	/// </summary>
	/// <returns>RGB do fundo</returns>
	RGB World::getBackgroundColor()
	{
		return _bgColor;
	}

	/// <summary>
	/// Obt�m a cor da luz ambiente
	/// </summary>
	/// <returns>A cor da luz ambiente em RGBA</returns>
	RGBA World::getAmbientLight()
	{
		return _ambientLightColor;
	}

	/// <summary>
	/// Obt�m a cor da luz difusa
	/// </summary>
	/// <returns>A cor da luz difusa em RGBA</returns>
	RGBA World::getDiffuseLight()
	{
		return _diffuseLightColor;
	}

	/// <summary>
	/// Muda a cor do fundo
	/// </summary>
	/// <param name="color">RGB</param>
	void World::setBackgroundColor(RGB color)
	{
		_bgColor = color;
	}

	/// <summary>
	/// Muda a cor da luz ambiente
	/// </summary>
	/// <param name="rgba">Cor RGBA</param>
	void World::setAmbientLight(RGBA rgba)
	{
		_ambientLightColor = rgba;
	}

	/// <summary>
	/// Muda a cor da luz difusa
	/// </summary>
	/// <param name="rgba">Cor RGBA</param>
	void World::setDiffuseLight(RGBA rgba)
	{
		_diffuseLightColor = rgba;
	}

	/// <summary>
	/// Obt�m a posi��o da luz
	/// </summary>
	/// <returns>A posi��o 4D da luz</returns>
	point4f World::getLightPosition()
	{
		return _lightOrigin;
	}

	/// <summary>
	/// Muda a posi��o da luz
	/// </summary>
	/// <param name="pos">Posi��o 4D</param>
	void World::setLightPosition(point4f pos)
	{
		_lightOrigin = pos;
	}
};