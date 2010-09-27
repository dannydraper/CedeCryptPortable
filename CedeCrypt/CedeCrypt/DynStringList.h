#include <windows.h>
#include <malloc.h>
#include <io.h>
#include <direct.h>
#include <stdio.h>

class DynStringList
{
	public:
		DynStringList ();
		~DynStringList ();
		void Clear ();
		void AddItem (LPCSTR szItem);
		LPCSTR GetItem (long iLoc);
		BOOL DoesExist (LPCSTR szItem);
		int GetNumItems ();
		void RemoveLastItem ();
	private:
		char *dynlist;
		BYTE *dynpointers;
		long dynlistsize;
		long dynpointerssize;
		long curpointer;
		long numitems;
};