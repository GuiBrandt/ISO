#include <iso.hpp>

using namespace Iso;

/// <summary>
/// Ponto de entrada do programa
/// </summary>
int main(void)
{
    try 
	{
		GameWindow window;
    
		Game::initialize();
		Input::initialize();
		Audio::initialize();

		while (!window.shouldClose())
			window.update();

		return 0;
	} 
	catch (int e) 
	{
		return e;
	}
}
