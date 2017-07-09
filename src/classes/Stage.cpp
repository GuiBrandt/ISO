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
	/// Construtor
	/// </summary>
	/// <param name="name">Nome do estágio</param>
	Stage::Stage(const char* name)
	{
		string fname = "data/stages/";
		fname += name;

		ifstream f(fname);
		string line;

		// Tamanho do mapa
		do getline(f, line);
		while (!isdigit(line[0]));

		int x, y, z;
		sscanf_s(line.c_str(), "%d %d %d", &x, &y, &z);

		_xSize = x;
		_ySize = y;
		_zSize = z;

		// Cor de fundo
		_bgColor = readHexColor(f);

		// Cor do jogador
		_playerColor = readHexColor(f);

		// Cor das luzes
		_ambientLightColor = readHexColor(f);
		_diffuseLightColor = readHexColor(f);
		_specularLightColor = readHexColor(f);

		// Posição das luzes
		do getline(f, line);
		while (!isdigit(line[0]));

		GLfloat fx, fy, fz;

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
		while (!isalnum(line[0]));

		do {
			char name[256];

			int color;
			sscanf_s(line.c_str(), "%d %d %d %255[^ ] %x", &x, &y, &z, name, 256, &color);

			_events.insert_or_assign(point3i { x, y, z }, Event(name, hexToRGB(color)));

			do getline(f, line);
			while (!(isalnum(line[0]) || line[0] == '$'));
		} while (line[0] != '$');
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