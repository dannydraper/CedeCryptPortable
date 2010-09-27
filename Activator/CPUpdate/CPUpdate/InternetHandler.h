#pragma once
#include <windows.h>
#include "MemoryBuffer.h"
#include "DynList.h"
#include "Wininet.h"

class InternetHandler
{
	public:
		InternetHandler ();
		~InternetHandler ();
		bool DoesWebConnectionExist ();
		bool InternetFiletoMemoryBuffer (char *szUrl, MemoryBuffer *memOut, unsigned int maxsize);
	private:


};