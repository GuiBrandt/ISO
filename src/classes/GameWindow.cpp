#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Evento de digitação com o teclado
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="key">Tecla pressionada</param>
	/// <param name="scan">Código de Scan do evento</param>
	/// <param name="action">Ação exercida</param>
	/// <param name="mods">Modificadores pressionados</param>
	void key_callback(GLFWwindow* w, int key, int scan, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			point3f pos = Game::getPlayer()->getPosition();

			switch (key)
			{
				// Movimento
				case GLFW_KEY_RIGHT:
					if (Game::getCurrentStage()->isPassable(pos.x + 1, pos.y, pos.z))
						Game::getPlayer()->move(1, 0, 0);
					break;
				case GLFW_KEY_UP:
					if (Game::getCurrentStage()->isPassable(pos.x, pos.y - 1, pos.z))
						Game::getPlayer()->move(0, -1, 0);
					break;
				case GLFW_KEY_LEFT:
					if (Game::getCurrentStage()->isPassable(pos.x - 1, pos.y, pos.z))
						Game::getPlayer()->move(-1, 0, 0);
					break;
				case GLFW_KEY_DOWN:
					if (Game::getCurrentStage()->isPassable(pos.x, pos.y + 1, pos.z))
						Game::getPlayer()->move(0, 1, 0);
					break;

				// Ação
				case GLFW_KEY_Z:
				case GLFW_KEY_SPACE:
					for (int z = -1; z <= 1; z++)
					for (int y = -1; y <= 1; y++)
					for (int x = -1; x <= 1; x++)
					{
						Event* ev = Game::getCurrentStage()->eventAt(pos.x + x, pos.y + y, pos.z + z);
						if (ev)
							ev->start();
					}

					break;
			}
		}
	}

	bool dragging = false;

	/// <summary>
	/// Evento de botão do mouse
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="button">Botão que disparou o evento</param>
	/// <param name="action">Ação realizada</param>
	/// <param name="mods">Modificadores</param>
	void mousebutton_callback(GLFWwindow* w, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			if (action == GLFW_PRESS)
				dragging = true;
			else
				dragging = false;
	}

	point2d cursorpos;

	double angle = 0.0;

	/// <summary>
	/// Evento de movimento do mouse
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="x">Posição X do mouse</param>
	/// <param name="y">Posição Y do mouse</param>
	void mousemove_callback(GLFWwindow* w, double x, double y)
	{
		if (dragging)
		{
			glRotatef(-45, 0, 0, 1);
			glRotatef(cursorpos.y - y, -1, 0, 0);
			angle += cursorpos.y - y;
			glRotatef(45, 0, 0, 1);
		}

		cursorpos = { x, y };
	}

	point2i windowpos = { 0, 0 };

	/// <summary>
	/// Evento de movimento da janela
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="x">Posição X da janela</param>
	/// <param name="y">Posição Y da janela</param>
	void windowmove_callback(GLFWwindow* w, int x, int y)
	{
		glRotatef(-45, 0, 0, 1);
		glRotatef(-angle, -1, 0, 0);

		glScalef(1 / .05, 1 / .05,  1);
		glTranslatef((windowpos.x - x) / (float)ISO_WINDOW_WIDTH * 2, (windowpos.y - y) / (float)ISO_WINDOW_HEIGHT * 2, 0);
		glScalef(.05, .05, 1);

		glRotatef(angle, -1, 0, 0);
		glRotatef(45, 0, 0, 1);

		Game::getWindow()->redraw();

		windowpos = { x, y };
	}

	/// <summary>
	/// Construtor
	/// </summary>
	GameWindow::GameWindow(void)
	{
		if (!glfwInit())
			throw -1;

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		_glfwWindow = glfwCreateWindow(
			ISO_WINDOW_WIDTH,
			ISO_WINDOW_HEIGHT,
			"",
			NULL,
			NULL
		);

		if (!_glfwWindow)
		{
			glfwTerminate();
			throw -2;
		}

		// Centraliza a janela
		point2i screenSize;
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		windowpos.x = mode->width / 2 - ISO_WINDOW_WIDTH / 2;
		windowpos.y = mode->height / 2 - ISO_WINDOW_HEIGHT / 2;

		glfwSetWindowPos(_glfwWindow, windowpos.x, windowpos.y);

		// Inicializa o OpenGL
		glfwMakeContextCurrent(_glfwWindow);

		// Callbacks
		glfwSetKeyCallback(_glfwWindow, key_callback);
		glfwSetMouseButtonCallback(_glfwWindow, mousebutton_callback);
		glfwSetCursorPosCallback(_glfwWindow, mousemove_callback);
		glfwSetWindowPosCallback(_glfwWindow, windowmove_callback);

		// Configurações do OpenGL
		glViewport(0, 0, ISO_WINDOW_WIDTH, ISO_WINDOW_HEIGHT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
				
		// Transformações
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(-1, 1, -1, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glScalef(.05, -.05, .05);
		glRotatef(45, 0, 0, 1);
		//glTranslatef(0, 0, .5);
		glPushMatrix();
	}

	/// <summary>
	/// Construtor
	/// </summary>
	GameWindow::~GameWindow(void)
	{
		glfwTerminate();
	}

	/// <summary>
	/// Verifica se a janela deveria fechar ou não
	/// </summary>
	/// <returns>True de o usuário tiver fechado a janela, false de não</returns>
	bool GameWindow::shouldClose(void)
	{
		return glfwWindowShouldClose(_glfwWindow);
	}

	/// <summary>
	/// Atualiza a janela do jogo
	/// </summary>
	void GameWindow::update(void)
	{
		redraw();
		glfwPollEvents();
	}

	/// <summary>
	/// Redesenha a tela
	/// </summary>
	void GameWindow::redraw(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Game::getCurrentStage()->render();

		glfwSwapBuffers(_glfwWindow);
	}

	/// <summary>
	/// Obtém a posição da janela
	/// </summary>
	/// <returns>Uma struct para a posição da janela</returns>
	point2i GameWindow::getPosition(void)
	{
		point2i p;
		glfwGetWindowPos(_glfwWindow, &p.x, &p.y);

		return p;
	}

	/// <summary>
	/// Define a posição da janela para um ponto
	/// </summary>
	/// <param name="x">Posição X para a janela</param>
	/// <param name="y">Posição Y para a janela</param>
	void GameWindow::setPosition(int x, int y)
	{
		glfwSetWindowPos(_glfwWindow, x, y);
	}

	/// <summary>
	/// Define o título da janela
	/// </summary>
	/// <param name="title">Título a ser atribuído à janela</param>
	void GameWindow::setTitle(const char *title)
	{
		glfwSetWindowTitle(_glfwWindow, title);
	}
};
