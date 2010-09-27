#pragma once
#include <windows.h>
#include <malloc.h>
#include <stdio.h>

class DynList {
	
	public:
		DynList ();
		~DynList ();		
		void AddItem (void *pItem, unsigned int itemSize, bool bUseAddress);
		void GetItem (void *dst, unsigned long iLoc);
		void *GetItem (unsigned long iLoc);
		void Clear ();
		unsigned int GetNumItems ();
	private:
		void ShowInt (int iInttoShow);

		BYTE *dynlist;
		BYTE *dynpointers;
		unsigned long dynlistsize;
		unsigned long dynpointerssize;
		unsigned long curpointer;
		unsigned long numitems;
		bool bCleared;
};
