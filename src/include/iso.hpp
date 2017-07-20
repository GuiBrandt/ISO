#pragma once
#include <map>
#include <vector>

#include <GLFW/glfw3.h>

#include <shiro.h>

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
/// Estrutura para um ponto 2D
/// </summary>
struct point2d
{
	double x;
	double y;
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
/// Estrutura para um ponto 4D
/// </summary>
struct point4f {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
};

/// <summary>
/// Estrutura para uma cor RGB
/// </summary>
struct RGB {
	GLclampf red;
	GLclampf green;
	GLclampf blue;
};

/// <summary>
/// Estrutura para uma cor RGBA
/// </summary>
struct RGBA {
	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
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

        public:
            GameWindow(void);
            ~GameWindow(void);

            bool shouldClose(void);
            void update(void);
			void redraw(void);

			point2i getPosition(void);
			void setPosition(int, int);

			void setTitle(const char*);
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
			static const float CHARACTER[];
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
	/// Classe para os eventos
	/// </summary>
	class Event : public GameObject
	{
		private:
			const char* _script;
			RGB _color;
			bool _visible;

		public:
			Event(const char*, RGB);
			~Event(void);

			void start(void);

			RGB getColor(void);
			void setColor(RGB);

			void show(void);
			void hide(void);

			bool isVisible();
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

			RGBA _ambientLightColor;
			RGBA _diffuseLightColor;

			point4f _lightOrigin;

			std::map<char, Tile> _tiles;
			std::map<point3i, Event*> _events;

			std::vector<MapLayer> _layers;

			const char* _password;
			const char* _next;

		public:
			Stage(const char*);
			~Stage(void);

			void render(void);
			bool isPassable(int, int, int);
			bool isEmpty(int, int, int);
			Event* eventAt(int, int, int);
			
			void setPassword(const char*, const char*);
			const char* getPassword(void);
			const char* getNext(void);
	};

	/// <summary>
	/// Classe principal do jogo
	/// </summary>
	class Game
	{
		private:
			static Player _player;
			static Stage* _currentStage;
			static Event* _currentEvent;
			static GameWindow* _window;
			static shiro_runtime* _scriptRuntime;

		public:
			static void initialize(void);
			static void terminate(void);

			static GameWindow* getWindow(void);

			static Player* getPlayer(void);

			static Stage* getCurrentStage(void);
			static void changeStage(const char*);

			static Event* getCurrentEvent();
			static void setCurrentEvent(Event*);

			static void runScript(const char*);
	};

	//
	//	Funções dos eventos
	//

	// Muda o título da janela
	shiro_native(setTitle);

	// Define a senha da fase
	shiro_native(setPass);

	// Muda de fase
	shiro_native(changeStage);

	// Mostra o evento atual
	shiro_native(show);
	
	// Esconde o evento atual
	shiro_native(hide);

	// Verifica se o evento atual está visível
	shiro_native(eventIsVisible);
};