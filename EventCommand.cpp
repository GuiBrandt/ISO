#include <iso.hpp>
#include <ctype.h>

namespace Iso
{
	/// <summary>
	/// Construtor
	/// </summary>
	/// <param name="command">Comando lido do arquivo do evento</param>
	EventCommand::EventCommand(const char* command)
	{
		// Tira os espaços do começo
		int i = 0;
		while (isspace(command[i])) i++;
		command += i;

		// Comentários e linhas em branco
		if (strlen(command) == 0 || command[0] == '#')
		{
			_code = "doNothing";
		}
		else
		{
			char* code = (char*)malloc(256);
			int index = strchr(command, ' ') - command;

			if (index < 0)
				throw - 1;

			strncat_s(code, 256, command, index);

			_code = code;
			_params = command + index;
		}
	}

	/// <summary>
	/// Destrutor
	/// </summary>
	EventCommand::~EventCommand()
	{
		if (_code)
			delete _code;

		if (_params)
			delete _params;
	}

	/// <summary>
	/// Obtém o código do comando
	/// </summary>
	/// <returns>O nome do comando</returns>
	const char* EventCommand::getCode()
	{
		return _code;
	}

	/// <summary>
	/// Executa o comando
	/// </summary>
	void EventCommand::execute()
	{
		//TODO
	}
};