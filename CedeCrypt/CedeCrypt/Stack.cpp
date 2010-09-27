#include "Stack.h"

Stack::Stack ()
{
	m_currentbuffer = -1;
	m_numitems = 0;
}

Stack::~Stack ()
{
}

void Stack::Push (unsigned int iValue)
{
	m_itemcount++;
	unsigned int val = iValue;

	if (m_currentbuffer == -1) {
		m_buffer1.SetSize (sizeof (unsigned int));
		m_buffer1.Append (&val, sizeof (unsigned int));
		m_currentbuffer = 1;
		m_numitems++;
	}
		
	if (m_currentbuffer == 1) {
		m_buffer2.SetSize (m_buffer1.GetSize () + sizeof (unsigned int));
		m_buffer2.Append (m_buffer1.GetBuffer (), m_buffer1.GetSize ());
		m_buffer2.Append (&val, sizeof (unsigned int));
		m_currentbuffer = 2;
		m_buffer1.Clear ();
		m_numitems++;
	}

	if (m_currentbuffer == 2) {
		m_buffer1.SetSize (m_buffer2.GetSize () + sizeof (unsigned int));
		m_buffer1.Append (m_buffer2.GetBuffer (), m_buffer2.GetSize ());
		m_buffer1.Append (&val, sizeof (unsigned int));
		m_currentbuffer = 1;
		m_buffer2.Clear ();
		m_numitems++;
	}
}

int Stack::Pop ()
{
	unsigned int val = 0;

	if (m_itemcount > 0) {
		m_itemcount--;
	}

	if (m_numitems > 1) {
		if (m_currentbuffer == 1) {
			memcpy (&val, m_buffer1.GetBuffer (), sizeof (unsigned int));
			m_numitems--;

			if (m_numitems > 0) {
				m_buffer2.SetSize (m_buffer1.GetSize ()-sizeof (unsigned int));
				m_buffer2.Write ((BYTE *) m_buffer1.GetBuffer ()+sizeof (unsigned int), 0, m_buffer1.GetSize ()-sizeof (unsigned int));
				m_buffer1.Clear ();
				m_currentbuffer = 2;
			} else {
				m_buffer2.Clear ();
				m_buffer1.Clear ();
				m_currentbuffer = -1;
			}
			
		}

		if (m_currentbuffer == 2) {
			memcpy (&val, m_buffer2.GetBuffer (), sizeof (unsigned int));
			m_numitems--;
			
			if (m_numitems > 0) {
				m_buffer1.SetSize (m_buffer2.GetSize ()-sizeof (unsigned int));
				m_buffer1.Write ((BYTE *) m_buffer2.GetBuffer ()+sizeof (unsigned int), 0, m_buffer2.GetSize ()-sizeof (unsigned int));
				m_buffer2.Clear ();
				m_currentbuffer = 1;
			} else {
				m_buffer2.Clear ();
				m_buffer1.Clear ();
				m_currentbuffer = -1;
			}
			
		}

		return val;
	} else {
		return -1;
	}
}

int Stack::GetNumItems ()
{
	return m_itemcount;
}
