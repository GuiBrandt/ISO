#include <iso.hpp>
#include <fstream>
#include <string>
#include <boxer.h>

using namespace std;

namespace Iso
{
	/// <summary>
	/// Processa uma camada de um mapa
	/// </summary>
	/// <param name="xSize">Tamanho X da camada</param>
	/// <param name="ySize">Tamanho Y da camada</param>
	/// <param name="map">Mapa da camada</param>
	/// <param name="ox">Offset X da camada</param>
	/// <param name="oy">Offset Y da camada</param>
	/// <param name="oz">Offset Z</param>
	void parseLayer(int xSize, int ySize, const char* map, GLfloat ox, GLfloat oy, GLfloat oz)
	{
		World* world = Game::getWorld();

		int x, y;

		for (y = 0; y < ySize; y++)
		for (x = 0; x < xSize; x++)
		{
			char t = map[x + y * xSize];
			Tile* tile = NULL;

			point3f pos = { ox + x, oy + y, oz };

			switch (t)
			{
				case '0':
					tile = new Tile(pos, RGB { 0.5f, 0.5f, 0.5f }, TileType::Floor);
					break;
				case 'x':
					tile = new Tile(pos, RGB { 1, 1, 1 }, TileType::Wall);
					break;
				case 'r':
					tile = new Tile(pos, RGB { 1, 0, 0 }, TileType::Wall);
					break;
				case 'g':
					tile = new Tile(pos, RGB { 0, 1, 0 }, TileType::Wall);
					break;
				case 'b':
					tile = new Tile(pos, RGB { 0, 0, 1 }, TileType::Wall);
					break;
				case 'y':
					tile = new Tile(pos, RGB { 1, 1, 0 }, TileType::Wall);
					break;
				case 'c':
					tile = new Tile(pos, RGB { 0, 1, 1 }, TileType::Wall);
					break;
				case 'p':
					tile = new Tile(pos, RGB { 1, 0, 1 }, TileType::Wall);
					break;
			}
			
			if (!tile)
				continue;

			world->add(tile);
		}
	}

	/// <summary>
	/// Carrega um mapa de um arquivo
	/// </summary>
	/// <param name="filename">Nome do arquivo</param>
	void loadMap(const char* filename)
	{
		string fname = "maps/";
		fname += filename;

		ifstream f(fname);
		string line;

		int xSize, ySize;

		getline(f, line);
		sscanf_s(line.c_str(), "%d %d", &xSize, &ySize);

		GLfloat fx, fy, fz;

		while (getline(f, line)) 
		{
			sscanf_s(line.c_str(), "%f %f %f", &fx, &fy, &fz);

			char* map = (char*)malloc(xSize * ySize + 1);
			memset(map, 0, xSize * ySize + 1);

			const char* row = line.c_str();

			for (int i = 0; i < ySize; i++)
			{
				getline(f, line);
				memcpy_s(map + i * xSize, xSize * ySize, row, strlen(row));
			}

			parseLayer(xSize, ySize, map, fx, fy, fz);
		}
	}

	/// <summary>
	/// Fase 4
	/// </summary>
	class Stage4 : public Stage {};

	/// <summary>
	/// Fase 3
	/// </summary>
	class Stage3 : public Stage
	{
		private:

			/// <summary>
			/// Evento 1
			/// </summary>
			class Event1 : public Event
			{
				public:
					/// <summary>
					/// Construtor
					/// </summary>
					Event1() : Event(point3f { 5, 5, 0 }, RGB { 1, 0, 0 }) {}

					/// <summary>
					/// Executa o evento
					/// </summary>
					void run() override
					{
						boxer::show("BAD", "E");
						exit(503);
					}
			};

			Event1 event1;

			int counter = 0;

		public:
			/// <summary>
			/// Inicialização
			/// </summary>
			void setup() override
			{
				loadMap("next");

				Game::getWindow()->setTitle("NOW, DON'T");

				World* world = Game::getWorld();
				world->setAmbientLight(RGBA{ 0.4f, 0.4f, 0.4f, 1 });
				world->setDiffuseLight(RGBA{ 0.02f, 0.02f, 0.02f, 0.1f });
				world->setLightPosition(point4f{ -12.8f, 16, -32, 0 });

				Player* player = Game::getPlayer();
				player->setColor(RGB{ 0.2f, 0.2f, 0.2f });
				player->setPosition(point3f{ 2, 2, 0 });
				world->add(player);

				world->add(&event1);
			}

			/// <summary>
			/// Atualização
			/// </summary>
			void update() override
			{
				counter++;

				if (counter > 600)
					Game::changeStage(new Stage4());
			}
	};

	/// <summary>
	/// Fase 2
	/// </summary>
	class Stage2 : public Stage
	{
		private:

			/// <summary>
			/// Evento 1
			/// </summary>
			class Event1 : public Event
			{
				public:
					/// <summary>
					/// Construtor
					/// </summary>
					Event1() : Event(point3f { 5, 5, 0 }, RGB { 0, 1, 0 }) {}

					/// <summary>
					/// Executa o evento
					/// </summary>
					void run() override
					{
						boxer::show("GOOD", "H");
						Game::changeStage(new Stage3());
					}
			};

			Event1 event1;

		public:
			/// <summary>
			/// Inicialização
			/// </summary>
			void setup() override
			{
				loadMap("next");

				Game::getWindow()->setTitle("TALK");

				World* world = Game::getWorld();
				world->setAmbientLight(RGBA{ 0.4f, 0.4f, 0.4f, 1 });
				world->setDiffuseLight(RGBA{ 0.02f, 0.02f, 0.02f, 0.1f });
				world->setLightPosition(point4f{ -12.8f, 16, -32, 0 });

				Player* player = Game::getPlayer();
				player->setColor(RGB { 0.2f, 0.2f, 0.2f });
				player->setPosition(point3f { 2, 2, 0 });
				world->add(player);

				world->add(&event1);
			}
	};

	/// <summary>
	/// Fase 1
	/// </summary>
	class Stage1 : public Stage
	{
		public:
			/// <summary>
			/// Inicialização
			/// </summary>
			void setup() override
			{
				loadMap("test");

				Game::getWindow()->setTitle("TYPE IN THE COLORS");

				World* world = Game::getWorld();
				world->setAmbientLight(RGBA { 0.4f, 0.4f, 0.4f, 1 });
				world->setDiffuseLight(RGBA { 0.02f, 0.02f, 0.02f, 0.1f });
				world->setLightPosition(point4f { -12.8f, 16, -32, 0 });

				Player* player = Game::getPlayer();
				player->setColor(RGB { 0.2f, 0.2f, 0.2f });
				player->setPosition(point3f { 2, 2, 0 });
				world->add(player);
			}

			/// <summary>
			/// Obtém a senha da fase
			/// </summary>
			const char* getPassword() override 
			{
				return "RGBYCP";
			}

			/// <summary>
			/// Se acertou a senha
			/// </summary>
			void onCorrect() override
			{
				Game::changeStage(new Stage2());
			}

			/// <summary>
			/// Se errou a senha
			/// </summary>
			/// <param name="attempt">Senha tentada pelo usuário</param>
			void onWrong(const char* attempt) override
			{
				if (strcmp(attempt, "KILLME") == 0)
				{
					Game::getWindow()->setTitle("AS YOU WILL");
					exit(666);
				}
			}
	};

	/// <summary>
	/// Obtém uma instância da primeira fase do jogo
	/// </summary>
	/// <returns>Um ponteiro para a primeira fase</returns>
	Stage* Game::getFirstStage()
	{
		return new Stage1();
	}
};