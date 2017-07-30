#include <iso.hpp>
#include <thread>

#include <ctype.h>

namespace Iso
{
	/// <summary>
	/// Buffer para o título da janela
	/// </summary>
	const char* old_title;

	/// <summary>
	/// Buffer para a sequência sendo escrita como senha para a próxima fase pelo jogador
	/// </summary>
	char writing[256];

	/// <summary>
	/// Índice do último caractere escrito na sequência
	/// </summary>
	size_t writing_i = 0;

	/// <summary>
	/// Ângulos de rotação da cena
	/// </summary>
	GLfloat angleX = 0, 
			angleY = 0, 
			angleZ = 0;

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
						Event* ev = Game::getWorld()->eventAt((int)pos.x + x, (int)pos.y + y, (int)pos.z + z);
						if (ev)
							ev->run();
					}

					break;

				case GLFW_KEY_X:
					if (angleX / 15.0 - floor(angleX / 15.0) != 0.0)
						return;

					for (int i = 0; i < 15; i++)
					{
						angleX += (mods & GLFW_MOD_CONTROL) ? 1 : -1;
						angleX = __min(__max(angleX, -90), 0);

						Game::getWindow()->update(false);
					}
					break;

				case GLFW_KEY_Z:
					if (angleZ / 15.0 - floor(angleZ / 15.0) != 0.0)
						return;

					for (int i = 0; i < 15; i++)
					{
						angleZ += (mods & GLFW_MOD_CONTROL) ? -1 : 1;

						Game::getWindow()->update(false);
					}
					break;
			}

			if (dir >= 0)
			{
				int x = 0, y = 0;

				if (angleZ < 0)
					angleZ = (GLfloat)(360 + (int)angleZ % 360);

				angleZ = (GLfloat)((int)angleZ % 360);
				
				dir += (int)floor((45 + angleZ) / 90);
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

				Game::getPlayer()->move(x, y);
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
				Game::getCurrentStage()->onCorrect();

				writing_i = 0;
				writing[writing_i] = 0;
			}
			else
			{
				Game::getCurrentStage()->onWrong(writing);

				glfwSetWindowTitle(w, "NOT THAT");
				std::this_thread::sleep_for(std::chrono::seconds(1));
				glfwSetWindowTitle(w, old_title);
				
				writing_i = 0;
				writing[writing_i] = 0;
			}
		}
	}

	/// <summary>
	/// Flag de arrasto da janela
	/// </summary>
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

	/// <summary>
	/// Posição do cursor na tela (não utilizado)
	/// </summary>
	point2d cursorpos;

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

	/// <summary>
	/// Posição da janela na tela
	/// </summary>
	point2i windowpos = { 0, 0 };

	/// <summary>
	/// Contador
	/// </summary>
	int dc = 0;

	/// <summary>
	/// Frases
	/// </summary>
	const char* phrases[] = { "STOP", "YOU DON'T CONTROL ME", "I'M TELLING YOU", "DIE"};

	/// <summary>
	/// Evento de movimento da janela
	/// </summary>
	/// <param name="w">Janela do jogo</param>
	/// <param name="x">Posição X da janela</param>
	/// <param name="y">Posição Y da janela</param>
	void windowmove_callback(GLFWwindow* w, int x, int y)
	{
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(w, mode->width / 2 - ISO_WINDOW_WIDTH / 2, mode->height / 2 - ISO_WINDOW_HEIGHT / 2);

		glfwSetWindowTitle(w, phrases[dc / 2000 % 4]);
		dc++;

		if (dc >= 8000)
			exit(666);

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
		glScalef(.05f * ISO_WINDOW_HEIGHT / ISO_WINDOW_WIDTH, -.05f, .05f);
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
	/// <param name="poll">Flag para tratar os eventos da janela</param>
	void GameWindow::update(bool poll)
	{
		dc = 0;

		if (writing_i == 0)
			glfwSetWindowTitle(_glfwWindow, old_title);
		else
			glfwSetWindowTitle(_glfwWindow, writing);

		Game::getCurrentStage()->update();
		Game::getWorld()->update();
		redraw();

		if (poll)
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

		Game::getWorld()->render();

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
