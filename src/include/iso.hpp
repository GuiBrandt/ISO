#pragma once
#include <GLFW/glfw3.h>

/// <summary>
/// Largura da janela do jogo
/// </summary>
#define ISO_WINDOW_WIDTH 640

/// <summary>
/// Altura da janela do jogo
/// </summary>
#define ISO_WINDOW_HEIGHT 480

namespace Iso
{
	/// <summary>
	/// Classe para a janela do jogo
	/// </summary>
    class GameWindow
    {
        private:
            GLFWwindow* _glfwWindowHandle;

        public:
            GameWindow(void);
            ~GameWindow(void);

            bool shouldClose(void);
            void update(void);
    };

	/// <summary>
	/// Classe para controle da entrada
	/// </summary>
    class Input
    {
        public:
            static void initialize(void);
    };

	/// <summary>
	/// Classe para controle da parte sonora do jogo
	/// </summary>
    class Audio
    {
        public:
            static void initialize(void);
    };

};
