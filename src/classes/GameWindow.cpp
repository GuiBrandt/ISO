#include <iso.hpp>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	GameWindow::GameWindow(void)
	{
		if (!glfwInit())
			throw -1;

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

		glViewport(0, 0, ISO_WINDOW_HEIGHT, ISO_WINDOW_HEIGHT);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnable(GL_COLOR_MATERIAL);
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

	int c = 0;

	/// <summary>
	/// Atualiza a janela do jogo
	/// </summary>
	void GameWindow::update(void)
	{
		glClearColor(_backgroundColor.red, _backgroundColor.green, _backgroundColor.blue, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		glLoadIdentity();
		glScalef(0.25, 0.25, 0.25);

		glTranslatef(-.5, 0, 0);

		glTranslatef(0.5, 0.5, -0.5);
		glRotatef(c++, 1, 1, 1);
		glTranslatef(-0.5, -0.5, 0.5);

		Game::getCurrentStage()->render();

		glPopMatrix();

		// Create light components
		GLfloat ambientLight[] = { .1, .1, .1, 1 };
		GLfloat diffuseLight[] = { 1, 1, 1, 1 };
		GLfloat specularLight[] = { .5, .5, .5, 1 };
		GLfloat position[] = { 0, 0, 0, 2 };

		// Assign created components to GL_LIGHT0
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		glfwSwapBuffers(_glfwWindow);
		glfwPollEvents();
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
	/// Define a cor de fundo da janela
	/// </summary>
	/// <param name="color">Cor com a qual o fundo da tela será pintado</param>
	void GameWindow::setBgColor(RGB color)
	{
		_backgroundColor = color;
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
