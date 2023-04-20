#include "stdafx.h"
#include "console.h"

UINT readFromConsole(TCHAR *buffer,int maxSize,bool echo/* = true*/)
{
	int charactersRead = 0;
	TCHAR one_character;
	DWORD onecharRead;

	ASSERT(maxSize > 0);

	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD consoleMode;
	GetConsoleMode(inputHandle,&consoleMode);
	
	consoleMode &= ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);

	SetConsoleMode(inputHandle,consoleMode);
	while(ReadConsole(inputHandle,&one_character,1,&onecharRead,NULL))
	{
		ASSERT(onecharRead == 1);
		switch(one_character)
		{
			case '\r':
			
				if(charactersRead < maxSize)
					buffer[charactersRead] = 0;
				return charactersRead;
			case 0x08:
				if(charactersRead > 0)
				{
					TCHAR backspace[] = _T("\x08 \x08");
					charactersRead--;
					WriteConsole(outputHandle,backspace,3,&onecharRead,NULL);
				}
				break;
			default:
				//if(/*isgraph(one_character) ||*/ one_character == ' ')
				{
					if(charactersRead < maxSize)
					{
						buffer[charactersRead++] = one_character;
						if(echo)
							WriteConsole(outputHandle,&one_character,1,&onecharRead,NULL);
					}					
				}
		}
	}
	return 0;
}
