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
		glClearColor(_backgroundColor.red, _backgroundColor.green, _backgroundColor.blue, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
