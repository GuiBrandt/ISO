#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Evento de digita��o com o teclado
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="key">Tecla pressionada</param>
	/// <param name="scan">C�digo de Scan do evento</param>
	/// <param name="action">A��o exercida</param>
	/// <param name="mods">Modificadores pressionados</param>
	void key_callback(GLFWwindow* w, int key, int scan, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			point3f pos = Game::getPlayer()->getPosition();
			switch (key)
			{
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
			}
		}
	}

	bool dragging = false;

	/// <summary>
	/// Evento de bot�o do mouse
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="button">Bot�o que disparou o evento</param>
	/// <param name="action">A��o realizada</param>
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

	/// <summary>
	/// Evento de movimento do mouse
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="x">Posi��o X do mouse</param>
	/// <param name="y">Posi��o Y do mouse</param>
	void mousemove_callback(GLFWwindow* w, double x, double y)
	{
		if (dragging)
		{
			glRotatef(-45, 0, 0, 1);
			glRotatef(cursorpos.y - y, -1, 0, 0);
			glRotatef(45, 0, 0, 1);
		}

		cursorpos = { x, y };
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

		glfwMakeContextCurrent(_glfwWindow);

		// Callbacks
		glfwSetKeyCallback(_glfwWindow, key_callback);
		glfwSetMouseButtonCallback(_glfwWindow, mousebutton_callback);
		glfwSetCursorPosCallback(_glfwWindow, mousemove_callback);

		// Configura��es do OpenGL
		glViewport(0, 0, ISO_WINDOW_HEIGHT, ISO_WINDOW_HEIGHT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		// Ilumina��o
		GLfloat ambientLight[] = { 0, 0, 0, 1 };
		GLfloat diffuseLight[] = { .1, .1, .1, .1 };
		//GLfloat position[] = { -.05, .7, -.9, 1 };
		GLfloat position[] = { -.1, .5, -1, 0 };

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
				
		// Transforma��es
		glScalef(.05, -.05, .05);
		glRotatef(45, 0, 0, 1);
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
	/// Verifica se a janela deveria fechar ou n�o
	/// </summary>
	/// <returns>True de o usu�rio tiver fechado a janela, false de n�o</returns>
	bool GameWindow::shouldClose(void)
	{
		return glfwWindowShouldClose(_glfwWindow);
	}

	int c = 0;

	/// <summary>
	/// Atualiza a janela do jogo
	/// </summary>
	void GameWindow::update(void)
	{
		glClearColor(_backgroundColor.red, _backgroundColor.green, _backgroundColor.blue, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Game::getCurrentStage()->render();

		glfwSwapBuffers(_glfwWindow);
		glfwPollEvents();
	}

	/// <summary>
	/// Obt�m a posi��o da janela
	/// </summary>
	/// <returns>Uma struct para a posi��o da janela</returns>
	point2i GameWindow::getPosition(void)
	{
		point2i p;
		glfwGetWindowPos(_glfwWindow, &p.x, &p.y);

		return p;
	}

	/// <summary>
	/// Define a posi��o da janela para um ponto
	/// </summary>
	/// <param name="x">Posi��o X para a janela</param>
	/// <param name="y">Posi��o Y para a janela</param>
	void GameWindow::setPosition(int x, int y)
	{
		glfwSetWindowPos(_glfwWindow, x, y);
	}

	/// <summary>
	/// Define a cor de fundo da janela
	/// </summary>
	/// <param name="color">Cor com a qual o fundo da tela ser� pintado</param>
	void GameWindow::setBgColor(RGB color)
	{
		_backgroundColor = color;
	}

	/// <summary>
	/// Define o t�tulo da janela
	/// </summary>
	/// <param name="title">T�tulo a ser atribu�do � janela</param>
	void GameWindow::setTitle(const char *title)
	{
		glfwSetWindowTitle(_glfwWindow, title);
	}
};
