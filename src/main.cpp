#include <iso.hpp>

using namespace Iso;

/// <summary>
/// Ponto de entrada do programa
/// </summary>
int main(void)
{
    try 
	{    
		Game::initialize();
		Input::initialize();
		Audio::initialize();

		GameWindow* window = Game::getWindow();
		while (!window->shouldClose())
			window->update();

		return 0;
	} 
	catch (int e) 
	{
		return e;
	}
}
