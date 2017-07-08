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

    _glfwWindowHandle = glfwCreateWindow(
        ISO_WINDOW_WIDTH,
        ISO_WINDOW_HEIGHT,
        "",
        NULL,
        NULL
    );

    if (!_glfwWindowHandle)
    {
        glfwTerminate();
        throw -2;
    }

    glfwMakeContextCurrent(_glfwWindowHandle);
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
    return glfwWindowShouldClose(_glfwWindowHandle);
}

/// <summary>
/// Atualiza a janela do jogo
/// </summary>
void GameWindow::update(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(_glfwWindowHandle);
    glfwPollEvents();
}

};
