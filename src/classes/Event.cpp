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

		loadCommands(name);
	}

	/// <summary>
	/// Carrega os comandos do evento do arquivo dele
	/// </summary>
	/// <param name="name">Nome do evento</param>
	void Event::loadCommands(const char* name)
	{
		string fname = "data/events/";
		fname += name;

		ifstream in(fname);
		string line;

		while (getline(in, line))
			_commands.push_back(new EventCommand(line.c_str()));
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
		
		for each (EventCommand* command in _commands)
			command->execute();
	}
};