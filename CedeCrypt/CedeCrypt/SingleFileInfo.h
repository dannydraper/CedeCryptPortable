#pragma once
#include <windows.h>
#include "resource.h"

class SingleFileInfo
{
	public:
		SingleFileInfo ();
		~SingleFileInfo ();	
		
		char szName[SIZE_STRING];
		char szFilepath[SIZE_STRING];
		char szRootdir[SIZE_STRING];
		char szFilesize[SIZE_NAME];
		char szDatemodified[SIZE_NAME];
		unsigned long lFilesize;
		bool bIsDir;

	private:
		///////////////// METHOD DEFINITIONS ///////////////////

};