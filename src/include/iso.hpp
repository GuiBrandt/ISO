#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <vector>

/// <summary>
/// Largura da janela do jogo
/// </summary>
#define ISO_WINDOW_WIDTH 480

/// <summary>
/// Altura da janela do jogo
/// </summary>
#define ISO_WINDOW_HEIGHT 480

/// <summary>
/// Estrutura para um ponto 2D
/// </summary>
struct point2i {
	int x;
	int y;
};

/// <summary>
/// Estrutura para um ponto 3D
/// </summary>
struct point3i {
	int x;
	int y;
	int z;

	bool operator<(const point3i &o) const {
		return x < o.x || (x == o.x && y < o.y) || (x == o.x && y == o.y && z < o.z);
	}
};

/// <summary>
/// Estrutura para um ponto 3D
/// </summary>
struct point3f {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

/// <summary>
/// Estrutura para uma cor RGB
/// </summary>
struct RGB {
	GLclampf red;
	GLclampf green;
	GLclampf blue;
};

namespace Iso
{
	/// <summary>
	/// Classe para a janela do jogo
	/// </summary>
    class GameWindow
    {
        private:
            GLFWwindow* _glfwWindow;
			RGB _backgroundColor = { 0, 0, 0 };

        public:
            GameWindow(void);
            ~GameWindow(void);

            bool shouldClose(void);
            void update(void);

			point2i getPosition(void);
			void setPosition(int, int);

			void setBgColor(RGB);

			void setTitle(const char*);
    };

	/// <summary>
	/// Classe para controle da entrada
	/// </summary>
    class Input
    {
        public:
            static void initialize(void);
    };

	/// <summary>
	/// Classe para controle da parte sonora do jogo
	/// </summary>
    class Audio
    {
        public:
            static void initialize(void);
    };

	/// <summary>
	/// Enumerador de tipos de Tile
	/// </summary>
	enum TileType
	{
		Wall  = 0,
		Floor = 1
	};

	/// <summary>
	/// Estrutura para uma especificação de Tile
	/// </summary>
	struct Tile
	{
		TileType type;
		RGB color;
	};

	/// <summary>
	/// Classe para camadas do mapa
	/// </summary>
	struct MapLayer
	{
		GLfloat originX;
		GLfloat originY;
		GLfloat originZ;

		char* map;
	};

	/// <summary>
	/// Classe genérica para os objetos do jogo
	/// </summary>
	class GameObject
	{
		protected:
			GLfloat _x;
			GLfloat _y;
			GLfloat _z;

		public:
			point3f getPosition(void);
			void move(GLfloat, GLfloat, GLfloat);
	};

	/// <summary>
	/// Classe para os modelos 3D do jogo
	/// </summary>
	class Models {
		public:
			static const float PLAYER[];
			static const float FLOOR[];
			static const float WALL[];
	};

	/// <summary>
	/// Classe para o jogador
	/// </summary>
	class Player : public GameObject
	{
	};

	/// <summary>
	/// Classe para os comandos dos eventos
	/// </summary>
	class EventCommand {
		private:
			const char* _code;
			const char* _params;

		public:
			EventCommand(const char*);

			const char* getCode(void);
			void execute(void);
	};

	/// <summary>
	/// Classe para os eventos
	/// </summary>
	class Event : public GameObject
	{
		private:
			std::vector<EventCommand*> _commands;
			RGB _color;

			void loadCommands(const char*);

		public:
			Event(const char*, RGB);

			void start(void);

			RGB getColor(void);
			void setColor(RGB);
	};

	/// <summary>
	/// Classe para um estágio do jogo
	/// </summary>
	class Stage
	{
		private:
			int _xSize;
			int _ySize;

			RGB _bgColor;
			RGB _playerColor;

			RGB _ambientLightColor;
			RGB _diffuseLightColor;
			RGB _specularLightColor;

			point3f _lightOrigin;

			std::map<char, Tile> _tiles;
			std::map<point3i, Event*> _events;

			std::vector<MapLayer> _layers;

		public:
			Stage(const char*);
			~Stage(void);

			void render(void);
			bool isPassable(int, int, int);
			Event* eventAt(int, int, int);
	};

	/// <summary>
	/// Classe principal do jogo
	/// </summary>
	class Game
	{
		private:
			static Player _player;
			static Stage _currentStage;
			static Event* _currentEvent;

		public:
			static void initialize();
			
			static Player* getPlayer();

			static Stage* getCurrentStage();
			static void changeStage(const char*);

			static Event* getCurrentEvent();
			static void setCurrentEvent(Event*);
	};
};