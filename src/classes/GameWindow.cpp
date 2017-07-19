#include <iso.hpp>
#include <thread>

namespace Iso
{
	GLfloat angleX, angleY, angleZ = 45;

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
				// Movimento
				case GLFW_KEY_RIGHT:
					if (Game::getCurrentStage()->isPassable(pos.x + 1, pos.y, pos.z))
						Game::getPlayer()->move(1, 0, 0);
					break;
				case GLFW_KEY_UP:
					if (mods & GLFW_MOD_CONTROL)
					{
						if (Game::getCurrentStage()->isPassable(pos.x, pos.y, pos.z - 1))
							Game::getPlayer()->move(0, 0, -1);
					}
					else if (Game::getCurrentStage()->isPassable(pos.x, pos.y - 1, pos.z))
						Game::getPlayer()->move(0, -1, 0);
					break;
				case GLFW_KEY_LEFT:
					if (Game::getCurrentStage()->isPassable(pos.x - 1, pos.y, pos.z))
						Game::getPlayer()->move(-1, 0, 0);
					break;
				case GLFW_KEY_DOWN:
					if (mods & GLFW_MOD_CONTROL)
					{
						if (Game::getCurrentStage()->isPassable(pos.x, pos.y, pos.z + 1))
							Game::getPlayer()->move(0, 0, 1);
					}
					else if (Game::getCurrentStage()->isPassable(pos.x, pos.y + 1, pos.z))
							Game::getPlayer()->move(0, 1, 0);
					break;

				// A��o
				case GLFW_KEY_ENTER:
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

				case GLFW_KEY_X:
					for (int i = 0; i < 15; i++)
					{
						angleX += (mods & GLFW_MOD_CONTROL) ? 1 : -1;

						int signal = angleX / abs(angleX);
						int val = __min(abs(angleX), 90);
						
						angleX = signal * val;

						Game::getWindow()->redraw();
					}
					break;
				case GLFW_KEY_Y:
					for (int i = 0; i < 15; i++)
					{
						angleY += (mods & GLFW_MOD_CONTROL) ? -1 : 1;

						int signal = angleY / abs(angleY);
						int val = __min(abs(angleY), 90);

						angleY = signal * val;

						Game::getWindow()->redraw();
					}
					break;
				case GLFW_KEY_Z:
					for (int i = 0; i < 15; i++)
					{
						angleZ += (mods & GLFW_MOD_CONTROL) ? -1 : 1;
						
						int signal = angleZ / abs(angleZ);
						int val = __min(abs(angleZ), 90);

						angleZ = signal * val;

						Game::getWindow()->redraw();
					}
					break;
			}
		}
	}

	const char* old_title;

	char writing[256];
	size_t writing_i = 0;

	/// <summary>
	/// Evento de digita��o
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="chr">Caractere digitado</param>
	void char_callback(GLFWwindow* w, unsigned int chr)
	{
		chr = toupper(chr);

		if (chr == 'X' || chr == 'Y' || chr == 'Z')
			return;

		writing[writing_i++] = chr;
		writing[writing_i] = 0;

		glfwSetWindowTitle(w, writing);

		if (writing_i == strlen(Game::getCurrentStage()->getPassword()))
		{
			if (strcmp(writing, Game::getCurrentStage()->getPassword()) == 0)
				Game::changeStage(Game::getCurrentStage()->getNext());
			else
			{
				glfwSetWindowTitle(w, "NOT THAT");
				std::this_thread::sleep_for(std::chrono::seconds(1));
				glfwSetWindowTitle(w, old_title);
				
				writing_i = 0;
				writing[writing_i] = 0;
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

	double angle = 0.0;

	/// <summary>
	/// Evento de movimento do mouse
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="x">Posi��o X do mouse</param>
	/// <param name="y">Posi��o Y do mouse</param>
	void mousemove_callback(GLFWwindow* w, double x, double y)
	{
		cursorpos = { x, y };
	}

	point2i windowpos = { 0, 0 };

	/// <summary>
	/// Evento de movimento da janela
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="x">Posi��o X da janela</param>
	/// <param name="y">Posi��o Y da janela</param>
	void windowmove_callback(GLFWwindow* w, int x, int y)
	{
		glScalef(1 / .05, 1 / .05,  1);
		glTranslatef((windowpos.x - x) / (float)ISO_WINDOW_WIDTH * 2, (windowpos.y - y) / (float)ISO_WINDOW_HEIGHT * 2, 0);
		glScalef(.05, .05, 1);

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
		glfwSetCharCallback(_glfwWindow, char_callback);
		glfwSetMouseButtonCallback(_glfwWindow, mousebutton_callback);
		glfwSetCursorPosCallback(_glfwWindow, mousemove_callback);
		glfwSetWindowPosCallback(_glfwWindow, windowmove_callback);

		// Configura��es do OpenGL
		glViewport(0, 0, ISO_WINDOW_WIDTH, ISO_WINDOW_HEIGHT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
				
		// Transforma��es
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glScalef(.05, -.05, .05);
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

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		glRotatef(angleX, 1, 0, 0);
		glRotatef(angleY, 0, 1, 0);
		glRotatef(angleZ, 0, 0, 1);

		Game::getCurrentStage()->render();

		glPopMatrix();

		glfwSwapBuffers(_glfwWindow);
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
	/// Define o t�tulo da janela
	/// </summary>
	/// <param name="title">T�tulo a ser atribu�do � janela</param>
	void GameWindow::setTitle(const char* title)
	{
		glfwSetWindowTitle(_glfwWindow, title);
		old_title = title;
	}
};
