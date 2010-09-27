#include "DynStringList.h"

DynStringList::DynStringList ()
{
	dynlistsize = 0;
	dynpointerssize = 0;
	curpointer = 0;
	numitems = 0;
}

DynStringList::~DynStringList ()
{
}

void DynStringList::Clear () {
	//if (dynlist != NULL) {
		free (dynlist);
	//	dynlist = NULL;
	//}
	//if (dynpointers != NULL) {
		free (dynpointers);
	//	dynpointers = NULL;
	//}
	dynlistsize = 0;
	dynpointerssize = 0;
	curpointer = 0;
	numitems = 0;
}

void DynStringList::AddItem (LPCSTR szItem) {

	// If the list size is greater than 0 then increase the size of the buffer
	// Add the next pointer
	if (dynpointerssize > 0) {
		// Reallocate memory for the next pointer
		dynpointers = (BYTE *)realloc (dynpointers, dynpointerssize + sizeof (long));
		memcpy (dynpointers + dynpointerssize, &curpointer, sizeof (long));
		dynpointerssize+=sizeof (long);
	}

	// Add the next string
	if (dynlistsize > 0) {
		dynlist = (char *)realloc (dynlist, dynlistsize + strlen (szItem));
		memcpy (dynlist + dynlistsize, szItem, strlen (szItem));
		dynlistsize+=strlen (szItem);
		curpointer+=strlen (szItem);
		numitems++;
	}
	
	// If the list size is 0, then allocate memory since it will not have been allocated.
	// Add the pointer
	if (dynpointerssize == 0) {
		// Allocate memory for the first time
		dynpointers = (BYTE *)calloc(sizeof (long), 1);
		memcpy (dynpointers + dynpointerssize, &curpointer, sizeof (long));
		dynpointerssize+=sizeof (long);
	}

	// Add the string
	if (dynlistsize == 0) {
		dynlist = (char *)calloc (strlen (szItem), 1);
		memcpy (dynlist + dynlistsize, szItem, strlen (szItem));
		dynlistsize+=strlen (szItem);
		curpointer+=strlen (szItem);
		numitems++;
	}

}

LPCSTR DynStringList::GetItem (long iLoc) {
	//printf ("\n NumItems: %d", numitems);
	if (iLoc < numitems) {
		
		long pointerloc = iLoc * sizeof (long);
		long dynloc = 0;
		long dynend = 0;
		long length = 0;	
		char szItem[1024];
		ZeroMemory (szItem, 1024);
		
		// Retrieve the list pointer for the given location
		memcpy (&dynloc, dynpointers + pointerloc, sizeof (long));
		
		// Get the end of the current item
		if (numitems-1 > iLoc) {
			memcpy (&dynend, dynpointers + ((iLoc + 1) * sizeof (long)), sizeof (long));
		} else {
			dynend = dynlistsize;
		}
		
		// Calculate the length of the current item.
		length = dynend - dynloc;
		
		// Retrieve the item from memory and put it in the local char string.
		//printf ("\n dynend: %d", dynend);
		//printf ("\n dynloc: %d", dynloc);
		//printf ("\n length: %d", length);
		memcpy (&szItem, dynlist + dynloc, length);
		//printf ("\n GI Got Here");
		return szItem;
	} else {
		return "";
	}
}

BOOL DynStringList::DoesExist (LPCSTR szItem) {
	int c = 0;
	
	for (c=0;c<numitems;c++) {
		//printf (" DE Got Here");
		//printf ("Value of C: %d", c);
		//printf ("\n szItem: ");
		//printf (szItem);
		//printf ("\n GetItem: ");
		//printf (GetItem (c));
		if (strcmp (szItem, GetItem (c)) == 0) {
			//printf (" DE Got Here 2");
			return TRUE;
		}
	}
	//printf (" DE Got Here 3");
	return FALSE;
}

int DynStringList::GetNumItems () {
	return numitems;
}

void DynStringList::RemoveLastItem () {
	if (numitems > 0) {
		char lastitem[1024];
		ZeroMemory (lastitem, 1024);
		int lastitemsize;
		strcpy (lastitem, GetItem (numitems-1));
		lastitemsize = strlen (lastitem);
		dynpointerssize-=sizeof (long);
		dynlistsize-=lastitemsize;
		curpointer-=lastitemsize;
		numitems--;
	}
}