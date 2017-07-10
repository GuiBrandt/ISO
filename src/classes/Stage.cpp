#include <iso.hpp>
#include <Models.hpp>

#include <fstream>
#include <string>
#include <ctype.h>

using namespace std;

namespace Iso
{
	/// <summary>
	/// Converte hexadecimal em RGB
	/// </summary>
	/// <param name="hex"></param>
	/// <returns></returns>
	RGB hexToRGB(int hex)
	{
		int r = (hex >> 16) & 0xff,
			g = (hex >> 8) & 0xff,
			b = hex & 0xff;

		return { r / 255.0f, g / 255.0f, b / 255.0f };
	}

	/// <summary>
	/// L� uma cor RGB a partir de hexadecimal no arquivo de est�gio
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
	/// Construtor
	/// </summary>
	/// <param name="name">Nome do est�gio</param>
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
		_ambientLightColor = readHexColor(f);
		_diffuseLightColor = readHexColor(f);
		_specularLightColor = readHexColor(f);

		// Posi��o das luzes
		do getline(f, line);
		while (!isdigit(line[0]));

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
		while (!isdigit(line[0]));

		do {
			char name[256];

			int color;
			sscanf_s(line.c_str(), "%d %d %d %255[^ ] %x", &x, &y, &z, name, 256, &color);

			_events.insert_or_assign(point3i { x, y, z }, new Event(name, hexToRGB(color)));

			do getline(f, line);
			while (!(isalnum(line[0]) || line[0] == '$'));
		} while (line[0] != '$');

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
	/// Renderiza um modelo em uma posi��o
	/// </summary>
	/// <param name="model">Modelo a ser desenhado</param>
	/// <param name="vertexCount">N�mero de v�rtices do modelo</param>
	/// <param name="color">Cor do modelo</param>
	/// <param name="position">Posi��o</param>
	/// <param name="mode">Modo de renderiza��o</param>
	void renderModel(const GLfloat* model, int vertexCount, RGB color, point3f position, GLenum mode = GL_TRIANGLES)
	{
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);

		glColor3fv((float*)&color);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), model);
		glNormalPointer(GL_FLOAT, 6 * sizeof(float), &model[3]);
		glDrawArrays(mode, 0, vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glPopMatrix();
	}

	/// <summary>
	/// Renderiza a cena na tela
	/// </summary>
	void Stage::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		// Jogador
		renderModel(Models::PLAYER, 24, _playerColor, Game::getPlayer()->getPosition(), GL_QUADS);
		
		glPopMatrix();
	}

	/// <summary>
	/// Verifica se uma posi��o � pass�vel
	/// </summary>
	/// <param name="x">X da posi��o</param>
	/// <param name="y">Y da posi��o</param>
	/// <param name="z">Z da posi��o</param>
	/// <returns>Verdadeiro se for e falso se n�o</returns>
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
				ev = pair.second;

		return ev;
	}
};