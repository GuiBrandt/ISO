#include <iso.hpp>

#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <future>

#include <boxer.h>

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
		shiro_push_arg_c(next, string, 1);

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
	/// Mostra uma mensagem
	/// </summary>
	shiro_native(say)
	{
		shiro_push_arg_c(msg, string, 0);
		shiro_push_arg_c(title, string, 1);
		shiro_push_arg_t(buttons, int, 2);
		shiro_push_arg_t(style, int, 3);

		return shiro_new_int((int)boxer::show(msg, title, (boxer::Style)style, (boxer::Buttons)buttons));
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

	// Muda a cor da luz ambiente
	shiro_native(setAmbientLight)
	{
		shiro_push_arg_c(r, float, 0);
		shiro_push_arg_c(g, float, 1);
		shiro_push_arg_c(b, float, 2);
		shiro_push_arg_c(a, float, 3);

		Game::getCurrentStage()->setAmbientLight(RGBA { (GLclampf)r, (GLclampf)g, (GLclampf)b, (GLclampf)a });

		return shiro_nil;
	}

	// Muda a cor da luz difusa
	shiro_native(setDiffuseLight)
	{
		shiro_push_arg_c(r, float, 0);
		shiro_push_arg_c(g, float, 1);
		shiro_push_arg_c(b, float, 2);
		shiro_push_arg_c(a, float, 3);

		Game::getCurrentStage()->setDiffuseLight(RGBA{ (GLclampf)r, (GLclampf)g, (GLclampf)b, (GLclampf)a });

		return shiro_nil;
	}

	// Muda a posição da luz
	shiro_native(setLightPosition)
	{
		shiro_push_arg_c(x, float, 0);
		shiro_push_arg_c(y, float, 1);
		shiro_push_arg_c(z, float, 2);
		shiro_push_arg_c(w, float, 3);

		Game::getCurrentStage()->setLightPosition(point4f { (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w });

		return shiro_nil;
	}

	// Converte RGBA em hexadecimal
	inline int rgbaToHex(RGBA rgba)
	{
		return ((int)(rgba.red * 255) << 24) + ((int)(rgba.green * 255) << 16) + ((int)(rgba.blue * 255) << 8) + (int)(rgba.alpha * 255);
	}

	// Obtém a cor da luz ambiente
	shiro_native(getAmbientLight)
	{
		return shiro_new_uint(rgbaToHex(Game::getCurrentStage()->getAmbientLight()));
	}

	// Obtém a cor da luz difusa
	shiro_native(getDiffuseLight)
	{
		return shiro_new_uint(rgbaToHex(Game::getCurrentStage()->getDiffuseLight()));
	}

	// Obtém a posição da luz
	shiro_native(getLightX)
	{
		return shiro_new_float(Game::getCurrentStage()->getLightPosition().x);
	}

	shiro_native(getLightY)
	{
		return shiro_new_float(Game::getCurrentStage()->getLightPosition().y);
	}

	shiro_native(getLightZ)
	{
		return shiro_new_float(Game::getCurrentStage()->getLightPosition().z);
	}

	shiro_native(getLightW)
	{
		return shiro_new_float(Game::getCurrentStage()->getLightPosition().y);
	}

	// Move o evento
	shiro_native(move)
	{
		shiro_push_arg_c(x, float, 0);
		shiro_push_arg_c(y, float, 1);
		shiro_push_arg_c(z, float, 2);

		Game::getCurrentEvent()->move(x, y, z);

		return shiro_nil;
	}

	// RIP
	shiro_native(die)
	{
		exit(1);

		return shiro_nil;
	}

	// Executa uma função em paralelo
	shiro_native(threadDo)
	{
		shiro_push_arg(func, 0);
		shiro_function* fn = get_func(func);

		std::thread::thread([](shiro_function* fn, shiro_runtime* runtime)
		{
			shiro_call_function(fn, runtime, 0);
		}, fn, runtime).detach();

		return shiro_nil;
	}

	// Define um timeout
	shiro_native(setTimeout)
	{
		shiro_push_arg_c(ms, int, 0);
		shiro_push_arg(func, 1);

		shiro_function* fn = get_func(func);

		std::thread::thread([](int ms, shiro_function* fn, shiro_runtime* runtime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			shiro_call_function(fn, runtime, 0);
		}, ms, fn, runtime).detach();

		return shiro_nil;
	}
};