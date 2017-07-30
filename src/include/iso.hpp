#pragma once
#include <map>
#include <vector>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

/// <summary>
/// Largura da janela do jogo
/// </summary>
#define ISO_WINDOW_WIDTH 640

/// <summary>
/// Altura da janela do jogo
/// </summary>
#define ISO_WINDOW_HEIGHT  640

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
            void update(bool = true);
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
	/// Classe para os modelos 3D do jogo
	/// </summary>
	class Models 
	{
		public:
			static const GLfloat CHARACTER[];
			static const GLfloat FLOOR[];
			static const GLfloat WALL[];
	};

	/// <summary>
	/// Classe genérica para os objetos do jogo
	/// </summary>
	class GameObject
	{
		protected:
			point3f _position;
			point3f _desiredPosition;
			RGB _color;
			bool _visible;

			const GLfloat* _model;
			int _vertexCount;
			
		public:
			GameObject(point3f, RGB, const GLfloat*, int);

			point3f getPosition(void);
			void setPosition(point3f);

			void move(int, int);
			void moveTo(int, int);

			virtual void onCollided(GameObject*) {}
			virtual void onHit(GameObject*) {}
			virtual void onInteraction(GameObject*) {}

			virtual bool isPassable(void) { return false; }
			virtual bool isPlayer(void) { return false; }
			virtual bool isEvent(void) { return false; }
			virtual bool isTile(void) { return false; }

			void update(void);

			const GLfloat* getModel(int&);

			RGB getColor(void);
			void setColor(RGB);

			void show(void);
			void hide(void);

			bool isVisible(void);
	};

	/// <summary>
	/// Enumerador de tipos de Tile
	/// </summary>
	enum TileType
	{
		Wall = 0,
		Floor = 1
	};

	/// <summary>
	/// Classe para os tiles
	/// </summary>
	class Tile : public GameObject {
		private:
			TileType _type;

		public:
			Tile(point3f, RGB, TileType);

			bool isPassable(void) override;
			bool isTile(void) override;
	};

	/// <summary>
	/// Classe para o jogador
	/// </summary>
	class Player : public GameObject {
		public:
			Player();

			bool isPlayer(void) override;
	};

	/// <summary>
	/// Classe para os eventos
	/// </summary>
	class Event : public GameObject {
		public:
			Event(point3f, RGB);

			bool isEvent(void) override;
			virtual void run() {};
	};

	/// <summary>
	/// Classe abstrata para um estágio do jogo
	/// </summary>
	class Stage
	{
		public:
			virtual void setup(void) {};
			virtual void update(void) {};
			virtual void onCorrect(void) {};
			virtual void onWrong(const char*) {};
			virtual const char* getPassword(void) { return NULL; };
	};

	/// <summary>
	/// Classe para o mundo do jogo
	/// </summary>
	class World
	{
		private:
			int _xSize;
			int _ySize;

			RGB _bgColor;

			RGBA _ambientLightColor;
			RGBA _diffuseLightColor;

			point4f _lightOrigin;

			std::vector<GameObject*> _objects;

		public:
			void add(GameObject*);
			void remove(GameObject*);
			void clear(void);

			void update(void);
			void render(void);
			bool isPassable(int, int, int, GameObject** = NULL);
			bool isEmpty(int, int, int, GameObject** = NULL);
			Event* eventAt(int, int, int);

			RGB getBackgroundColor(void);
			void setBackgroundColor(RGB);

			RGBA getAmbientLight(void);
			void setAmbientLight(RGBA);
			RGBA getDiffuseLight(void);
			void setDiffuseLight(RGBA);

			point4f getLightPosition(void);
			void setLightPosition(point4f);
	};

	/// <summary>
	/// Classe principal do jogo
	/// </summary>
	class Game
	{
		private:
			static Player _player;
			static Stage* _currentStage;
			static World* _world;
			static Event* _currentEvent;
			static GameWindow* _window;

		public:
			static void initialize(void);
			static void terminate(void);

			static GameWindow* getWindow(void);

			static Player* getPlayer(void);

			static World* getWorld(void);

			static Stage* getCurrentStage(void);
			static Stage* getFirstStage(void);
			static void changeStage(Stage*);

			static Event* getCurrentEvent();
			static void setCurrentEvent(Event*);
	};
};