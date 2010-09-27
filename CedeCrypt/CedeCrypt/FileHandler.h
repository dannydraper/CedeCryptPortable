#pragma once

#include <windows.h>
#include "Diagnostics.h"

class FileHandler
{
	public:
		FileHandler ();
		~FileHandler ();				

		bool FileExists (char *FileName);
		bool CheckDuplicatedFile (char *szDestpath, char *szNewDestpath);
		void SplitFileParts (char *szInpath, char *szOutNoext, char *szExtonly);
		


	private:
		///////////////// METHOD DEFINITIONS ///////////////////

};