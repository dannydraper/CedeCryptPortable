#include "SingleFileInfo.h"

SingleFileInfo::SingleFileInfo()
{
	ZeroMemory (szName, SIZE_STRING);
	ZeroMemory (szFilepath, SIZE_STRING);
	ZeroMemory (szFilesize, SIZE_NAME);
	ZeroMemory (szDatemodified, SIZE_NAME);
	lFilesize = 0;
	bIsDir = false;
}

SingleFileInfo::~SingleFileInfo()
{

}
