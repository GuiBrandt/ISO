#include <iso.hpp>

#include <fstream>
#include <string>
#include <ctype.h>

using namespace std;

namespace Iso
{
	/// <summary>
	/// Converte hexadecimal em RGB
	/// </summary>
	/// <param name="hex">Hexadecimal</param>
	/// <returns>RGB</returns>
	RGB hexToRGB(int hex)
	{
		int r = (hex >> 16) & 0xff,
			g = (hex >> 8) & 0xff,
			b = hex & 0xff;

		return { r / 255.0f, g / 255.0f, b / 255.0f };
	}

	/// <summary>
	/// Converte hexadecimal em RGBA
	/// </summary>
	/// <param name="hex">Hexadecimal</param>
	/// <returns>RGBA</returns>
	RGBA hexToRGBA(int hex)
	{
		int r = (hex >> 24) & 0xff,
			g = (hex >> 16) & 0xff,
			b = (hex >> 8) & 0xff,
			a = hex & 0xff;

		return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
	}

	/// <summary>
	/// Lê uma cor RGB a partir de hexadecimal no arquivo de estágio
	/// </summary>
	/// <param name="in">Stream de leitura do arquivo</param>
	/// <returns>Um RGB com a cor lida</returns>
	RGB readHexColor(ifstream& in)
	{
		string line;

		do getline(in, line); 
		while (!isxdigit(line[0]));

		int hex;
		sscanf_s(line.c_str(), "%06x", &hex);

		return hexToRGB(hex);
	}

	/// <summary>
	/// Lê uma cor RGBA a partir de hexadecimal no arquivo de estágio
	/// </summary>
	/// <param name="in">Stream de leitura do arquivo</param>
	/// <returns>Um RGB com a cor lida</returns>
	RGBA readHexColor4(ifstream& in)
	{
		string line;

		do getline(in, line);
		while (!isxdigit(line[0]));

		int hex;
		sscanf_s(line.c_str(), "%08x", &hex);

		return hexToRGBA(hex);
	}

	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="name">Nome do estágio</param>
	Stage::Stage(const char* name)
	{
		string fname = "data/stages/";
		fname += name;

		ifstream f(fname);
		string line;

		int x, y, z;
		GLfloat fx, fy, fz;

		// Tamanho do mapa
		do getline(f, line);
		while (!isdigit(line[0]));

		sscanf_s(line.c_str(), "%d %d", &x, &y);

		_xSize = x;
		_ySize = y;

		// Cor de fundo
		_bgColor = readHexColor(f);

		// Cor do jogador
		_playerColor = readHexColor(f);

		// Cor das luzes
		_ambientLightColor = readHexColor4(f);
		_diffuseLightColor = readHexColor4(f);

		// Posição das luzes
		do getline(f, line);
		while (!(isdigit(line[0]) || line[0] == '-'));

		sscanf_s(line.c_str(), "%f %f %f", &fx, &fy, &fz);
		
		_lightOrigin = { fx, fy, fz };

		// Tiles
		do getline(f, line);
		while (!isalnum(line[0]));

		do {
			char c;
			int type, color;
			sscanf_s(line.c_str(), "%c %d %x", &c, 1, &type, &color);

			_tiles.insert_or_assign(c, Tile{ (TileType)type, hexToRGB(color) });

			do getline(f, line);
			while (!(isalnum(line[0]) || line[0] == '$'));
		} while (line[0] != '$');

		// Eventos
		do getline(f, line);
		while (!(isdigit(line[0]) || line[0] == '-' || line[0] == '$'));

		while (line[0] != '$') 
		{
			char name[256];

			int color;
			sscanf_s(line.c_str(), "%d %d %d %255[^ ] %x", &x, &y, &z, name, 256, &color);

			_events.insert_or_assign(point3i { x, y, -z }, new Event(name, hexToRGB(color)));

			do getline(f, line);
			while (!(isalnum(line[0]) || line[0] == '$'));
		}

		// Mapa
		do getline(f, line);
		while (!isdigit(line[0]));

		do {
			sscanf_s(line.c_str(), "%f %f %f", &fx, &fy, &fz);

			MapLayer layer;
			layer.originX = fx;
			layer.originY = fy;
			layer.originZ = fz;
			layer.map = (char*)malloc(_xSize * _ySize + 1);
			memset(layer.map, 0, _xSize * _ySize + 1);

			const char* row = line.c_str();

			for (int i = 0; i < _ySize; i++)
			{
				getline(f, line);
				memcpy_s(layer.map + i * _xSize, _xSize * _ySize, row, strlen(row));
			}

			_layers.push_back(layer);

			getline(f, line);
		} while (line[0] != '$');
	}

	/// <summary>
	/// Destrutor
	/// </summary>
	Stage::~Stage()
	{
		for each (MapLayer layer in _layers)
			delete layer.map;
	}

	/// <summary>
	/// Renderiza um modelo em uma posição
	/// </summary>
	/// <param name="model">Modelo a ser desenhado</param>
	/// <param name="vertexCount">Número de vértices do modelo</param>
	/// <param name="color">Cor do modelo</param>
	/// <param name="position">Posição</param>
	/// <param name="mode">Modo de renderização</param>
	void renderModel(const GLfloat* model, int vertexCount, RGB color, point3f position, GLenum mode = GL_TRIANGLES)
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
	/// Renderiza a cena na tela
	/// </summary>
	void Stage::render(void)
	{
		glClearColor(_bgColor.red, _bgColor.green, _bgColor.blue, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// Iluminação
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

		// Jogador
		renderModel(Models::CHARACTER, 24, _playerColor, Game::getPlayer()->getPosition(), GL_QUADS);
		
		// Mapa
		for (unsigned int i = 0; i < _layers.size(); i++)
		{
			MapLayer layer = _layers[i];
			int z = -layer.originZ;

			for (int x = 0; x < _xSize; x++)
			for (int y = 0; y < _ySize; y++)
			{
				char tile = layer.map[y * _xSize + x];

				if (!_tiles.count(tile))
					continue;

				Tile t = _tiles[tile];
				
				switch (t.type)
				{
					case TileType::Floor:
						renderModel(Models::FLOOR, 8, t.color, point3f { (float)x, (float)y, (float)z }, GL_QUADS);
						break;

					case TileType::Wall:
						renderModel(Models::WALL, 24, t.color, point3f { (float)x, (float)y, (float)z }, GL_QUADS);
						break;
				}
			}
		}

		// Eventos
		for each (auto pair in _events)
			if (pair.second->isVisible())
				renderModel(Models::CHARACTER, 24, pair.second->getColor(), point3f{ (float)pair.first.x, (float)pair.first.y, (float)pair.first.z }, GL_QUADS);

		glPopMatrix();
	}

	/// <summary>
	/// Verifica se uma posição é passável
	/// </summary>
	/// <param name="x">X da posição</param>
	/// <param name="y">Y da posição</param>
	/// <param name="z">Z da posição</param>
	/// <returns>Verdadeiro se for e falso se não</returns>
	bool Stage::isPassable(int x, int y, int z)
	{
		for each (MapLayer layer in _layers)
		{
			int oz = -round(layer.originZ);

			char tile = layer.map[y * _xSize + x];

			if (!_tiles.count(tile))
				continue;

			Tile t = _tiles[tile];

			if (z == oz && t.type == Wall)
				return false;
		}

		for each (auto pair in _events)
			if (pair.first.x == x && pair.first.y == y && pair.first.z == z && pair.second->isVisible())
				return false;

		return true;
	}

	/// <summary>
	/// Verifica se uma posição está sem tiles
	/// </summary>
	/// <param name="x">X da posição</param>
	/// <param name="y">Y da posição</param>
	/// <param name="z">Z da posição</param>
	/// <returns>Verdadeiro se estiver e falso se não</returns>
	bool Stage::isEmpty(int x, int y, int z)
	{
		for each (MapLayer layer in _layers)
		{
			int oz = -round(layer.originZ);

			char tile = layer.map[y * _xSize + x];

			if (oz == z && _tiles.count(tile))
				return false;
		}

		return true;
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
				ev = pair.second;

		return ev;
	}

	/// <summary>
	/// Determina a senha para passar de fase
	/// </summary>
	/// <param name="pass">Senha</param>
	/// <param name="next">Nome da próxima fase</param>
	void Stage::setPassword(const char* pass, const char* next = NULL)
	{
		_password = pass;

		if (next != NULL)
			_next = next;
	}

	/// <summary>
	/// Obtém a senha para passar de fase
	/// </summary>
	const char* Stage::getPassword()
	{
		return _password;
	}

	/// <summary>
	/// Obtém o nome da próxima fase
	/// </summary>
	const char* Stage::getNext()
	{
		return _next;
	}
};