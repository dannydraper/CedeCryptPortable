#include "DynList.h"

DynList::DynList ()
{
	dynlistsize = 0;
	dynpointerssize = 0;
	curpointer = 0;
	numitems = 0;
	bCleared = true;
}

DynList::~DynList ()
{
	
}

void DynList::ShowInt (int iInttoShow) {
	char szMsg[255];
	ZeroMemory (szMsg, 255);
	sprintf_s (szMsg, 255, "Value of int: %d", iInttoShow);
	MessageBox (NULL, szMsg, "ShowInt", MB_OK);
}

void DynList::Clear () {

	if (bCleared == false) {
		free (dynlist);
		free (dynpointers);
		bCleared = true;
	}
	
	dynlistsize = 0;
	dynpointerssize = 0;
	curpointer = 0;
	numitems = 0;
}

unsigned int DynList::GetNumItems () {
	return numitems;
}

void DynList::AddItem (void *pItem, unsigned int itemSize, bool bUseAddress)
{
	bCleared = false; // Set the cleared flag to false.

	// If the list size is greater than 0 then increase the size of the buffer
	// Add the next buffer pointer
	if (dynpointerssize > 0) {
		// Reallocate memory for the next pointer
		dynpointers = (BYTE *) realloc (dynpointers, dynpointerssize + sizeof (unsigned long));
		memcpy (dynpointers + dynpointerssize, &curpointer, sizeof (unsigned long));
		dynpointerssize+=sizeof (unsigned long);
	}

	// Add the next requested item
	if (dynlistsize > 0) {
		dynlist = (BYTE *) realloc (dynlist, dynlistsize + itemSize);
		if (bUseAddress == true) {
			memcpy (dynlist + dynlistsize, &pItem, itemSize);
		} else {
			memcpy (dynlist + dynlistsize, pItem, itemSize);		
		}
		dynlistsize+=itemSize;
		curpointer+=itemSize;
		numitems++;
	}

	// If the list size is 0, then allocate memory since it will not have been allocated.
	// First add the pointer to the buffer.
	if (dynpointerssize == 0) {
		// Allocate initial memory buffer
		dynpointers = (BYTE *) calloc (sizeof (unsigned long), 1);
		memcpy (dynpointers + dynpointerssize, &curpointer, sizeof (unsigned long));
		dynpointerssize+=sizeof (unsigned long);
	}

	// Add the requested item to the buffer
	if (dynlistsize == 0) {
		dynlist = (BYTE *) calloc (itemSize, 1);
		if (bUseAddress == true) {
			memcpy (dynlist + dynlistsize, &pItem, itemSize);
		} else {
			memcpy (dynlist + dynlistsize, pItem, itemSize);
		}
		dynlistsize+=itemSize;
		curpointer+=itemSize;
		numitems++;
	}
}

void DynList::GetItem (void *dst, unsigned long iLoc)
{
	if (iLoc <numitems) {
		unsigned long pointerloc = iLoc * sizeof (unsigned long);
		unsigned long dynloc = 0;
		unsigned long dynend = 0;
		unsigned long length = 0;

		// Retrieve the list pointer for the given location
		memcpy (&dynloc, dynpointers + pointerloc, sizeof (unsigned long));

		// Get the end of the current item
		if (numitems-1 > iLoc) {
			memcpy (&dynend, dynpointers + ((iLoc + 1) * sizeof (unsigned long)), sizeof (unsigned long));
		} else {
			dynend = dynlistsize;
		}

		// Calculate the length of the current item
		length = dynend - dynloc;

		// Now retrieve the requested item, and place into provided buffer
		memcpy (dst, dynlist + dynloc, length);
	}
}

void *DynList::GetItem (unsigned long iLoc)
{
	if (iLoc < numitems) {
		unsigned long pointerloc = iLoc * sizeof (unsigned long);
		unsigned long dynloc = 0;

		// Retrieve the list pointer for the given location
		memcpy (&dynloc, dynpointers + pointerloc, sizeof (unsigned long));

		// Now return a pointer in memory to the requested item
		return (void *) (dynlist+dynloc);
	} else {
		return NULL;
	}
}