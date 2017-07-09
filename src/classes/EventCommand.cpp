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
			_code = "doNothing";
		else
		{
			int index = strchr(command, ' ') - command;

			if (index > 0)
			{
				char* code = (char*)calloc(256, sizeof(char));
				strncat_s(code, 256, command, index);

				_code = code;
				_params = command + index;
			}
			else
				_code = command;
		}
	}

	/// <summary>
	/// Destrutor
	/// </summary>
	EventCommand::~EventCommand()
	{
		/*if (_code)
			delete _code;

		if (_params)
			delete _params;*/
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