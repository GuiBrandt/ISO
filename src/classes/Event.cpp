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

		show();
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

	/// <summary>
	/// Mostra o evento
	/// </summary>
	void Event::show()
	{
		_visible = true;
	}

	/// <summary>
	/// Esconde o evento
	/// </summary>
	void Event::hide()
	{
		_visible = false;
	}

	/// <summary>
	/// Verifica se o evento é visível
	/// </summary>
	/// <returns></returns>
	bool Event::isVisible()
	{
		return _visible;
	}

	/// <summary>
	/// Muda o título da janela do jogo
	/// </summary>
	shiro_native(setTitle)
	{
		shiro_push_arg_c(title, string, 0);

		Game::getWindow()->setTitle(title);

		return shiro_nil;
	}

	/// <summary>
	/// Define a senha para passar de fase
	/// </summary>
	shiro_native(setPass)
	{
		shiro_push_arg_c(pass, string, 0);
		shiro_push_arg_c(next, string, 0);

		Game::getCurrentStage()->setPassword(pass, next);

		return shiro_nil;
	}

	/// <summary>
	/// Muda a fase atual
	/// </summary>
	shiro_native(changeStage)
	{
		shiro_push_arg_c(name, string, 0);

		Game::changeStage(name);

		return shiro_nil;
	}

	/// <summary>
	/// Mostra o evento atual
	/// </summary>
	shiro_native(show)
	{
		Game::getCurrentEvent()->show();
		return shiro_nil;
	}

	/// <summary>
	/// Esconde o evento atual
	/// </summary>
	shiro_native(hide)
	{
		Game::getCurrentEvent()->hide();
		return shiro_nil;
	}

	/// <summary>
	/// Verifica se o evento atual está visível
	/// </summary>
	shiro_native(eventIsVisible)
	{
		return shiro_new_int(Game::getCurrentEvent()->isVisible());
	}

};