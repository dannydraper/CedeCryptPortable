#include "MemoryBuffer.h"
#include "resource.h"

class Stack {
	
	public:
		Stack ();
		~Stack ();
		void Push (unsigned int iValue);
		int Pop ();
		int GetNumItems ();
	private:
		
		int m_currentbuffer;
		MemoryBuffer m_buffer1;
		MemoryBuffer m_buffer2;
		int m_numitems;
		int m_itemcount;
};