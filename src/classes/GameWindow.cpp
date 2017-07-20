#include <iso.hpp>
#include <thread>

namespace Iso
{
	const char* old_title;
	char writing[256];
	size_t writing_i = 0;

	GLfloat angleX, angleY, angleZ = 45;

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

			int dir = -1;

			switch (key)
			{
				case GLFW_KEY_BACKSPACE:
					if (writing_i > 0)
					{
						writing[--writing_i] = 0;
						if (writing_i > 0)
							glfwSetWindowTitle(w, writing);
						else
							glfwSetWindowTitle(w, old_title);
					}
					break;

				// Movimento
				case GLFW_KEY_RIGHT:
					dir = 0;
					break;
				case GLFW_KEY_UP:
					dir = 1;
					break;
				case GLFW_KEY_LEFT:
					dir = 2;
					
					break;
				case GLFW_KEY_DOWN:
					dir = 3;
					break;

				// Ação
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
					if (angleX / 15.0 - floor(angleX / 15.0) != 0.0)
						return;

					for (int i = 0; i < 15; i++)
					{
						angleX += (mods & GLFW_MOD_CONTROL) ? 1 : -1;
						angleX = __min(__max(angleX, -90), 0);

						Game::getWindow()->update();
					}
					break;

				case GLFW_KEY_Z:
					if (angleZ / 15.0 - floor(angleZ / 15.0) != 0.0)
						return;

					for (int i = 0; i < 15; i++)
					{
						angleZ += (mods & GLFW_MOD_CONTROL) ? -1 : 1;

						Game::getWindow()->update();
					}
					break;
			}

			if (dir >= 0)
			{
				int x = 0, y = 0;

				if (angleZ < 0)
					angleZ = 360 + (int)angleZ % 360;

				angleZ = (int)angleZ % 360;
				
				dir += floor((45 + angleZ) / 90);
				dir %= 4;

				switch (dir)
				{
					case 0:
						x = 1;
						break;
					case 1:
						y = -1;
						break;
					case 2:
						x = -1;
						break;
					case 3:
						y = 1;
						break;
				}

				int z = 0;

				if (!Game::getCurrentStage()->isEmpty(pos.x + x, pos.y + y, pos.z))
				{
					if (Game::getCurrentStage()->isPassable(pos.x + x, pos.y + y, pos.z))
						z = 0;
					else if (Game::getCurrentStage()->isPassable(pos.x + x, pos.y + y, pos.z + 1) && Game::getCurrentStage()->isPassable(pos.x + x, pos.y + y, pos.z))
						z = 1;
					else if (Game::getCurrentStage()->isPassable(pos.x + x, pos.y + y, pos.z - 1) && Game::getCurrentStage()->isPassable(pos.x, pos.y, pos.z - 1))
						z = -1;
					else
						return;
				}
				else while (Game::getCurrentStage()->isPassable(pos.x + x, pos.y + y, pos.z + z + 1) && Game::getCurrentStage()->isEmpty(pos.x + x, pos.y + y, pos.z + z) && z < 256)
					z++;

				float speed = 0.25f;
				int i;

				if (z < 0)
					for (i = 0; i < 1 / speed; i++)
					{
						Game::getPlayer()->move(0, 0, z * speed);
						Game::getWindow()->redraw();
					}

				for (i = 0; i < 1 / speed; i++)
				{
					Game::getPlayer()->move(x * speed, y * speed, 0);
					Game::getWindow()->redraw();
				}

				if (z > 0)
					for (i = 0; i < z / speed; i++)
					{
						Game::getPlayer()->move(0, 0, speed);
						Game::getWindow()->redraw();
					}
			}
		}
	}

	/// <summary>
	/// Evento de digitação
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="chr">Caractere digitado</param>
	void char_callback(GLFWwindow* w, unsigned int chr)
	{
		if (Game::getCurrentStage()->getPassword() == NULL)
			return;

		chr = toupper(chr);

		if (chr == 'X' || chr == 'Z' || chr == ' ')
			return;

		writing[writing_i++] = chr;
		writing[writing_i] = 0;

		glfwSetWindowTitle(w, writing);

		if (writing_i == strlen(Game::getCurrentStage()->getPassword()))
		{
			if (strcmp(writing, Game::getCurrentStage()->getPassword()) == 0)
			{
				Game::changeStage(Game::getCurrentStage()->getNext());
				Game::getPlayer()->moveTo(2, 2, 0);

				writing_i = 0;
				writing[writing_i] = 0;
			}
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
		glScalef(1 / .05, 1 / .05,  1);
		glTranslatef((windowpos.x - x) / (float)ISO_WINDOW_WIDTH * 2, (windowpos.y - y) / (float)ISO_WINDOW_HEIGHT * 2, 0);
		glScalef(.05, .05, 1);

		Game::getWindow()->update();

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
	/// Verifica se a janela deveria fechar ou não
	/// </summary>
	/// <returns>True se o usuário tiver fechado a janela, false de não</returns>
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
	void GameWindow::setTitle(const char* title)
	{
		glfwSetWindowTitle(_glfwWindow, title);
		old_title = title;
	}
};
