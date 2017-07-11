#include <iso.hpp>
#include <fstream>
#include <string>

using namespace std;

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="name">Nome do evento</param>
	/// <param name="color">Cor para o evento</param>
	Event::Event(const char* name, RGB color)
	{
		_color = color;

		string script, line;

		string fname = "data/events/";
		fname += name;
		ifstream file(fname);
		
		while (getline(file, line))
			script.append(line + "\n");

		char* buffer = (char*)calloc(script.length() + 1, sizeof(char));
		script._Copy_s(buffer, script.length() + 1, script.length());

		_script = buffer;
	}

	/// <summary>
	/// Destrutor
	/// </summary>
	Event::~Event()
	{
		delete _script;
	}

	/// <summary>
	/// Obtém a cor do evento
	/// </summary>
	/// <returns>RGB do evento</returns>
	RGB Event::getColor()
	{
		return _color;
	}

	/// <summary>
	/// Define a cor do evento
	/// </summary>
	void Event::setColor(RGB rgb)
	{
		_color = rgb;
	}

	/// <summary>
	/// Inicia o evento
	/// </summary>
	void Event::start()
	{
		Game::setCurrentEvent(this);
		Game::runScript(_script);
	}
};