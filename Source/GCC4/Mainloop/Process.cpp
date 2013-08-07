

#include "GameCodeStd.h"
#include "Process.h"



//---------------------------------------------------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------------------------------------------------
Process::Process(void)
{
	m_state = UNINITIALIZED;
	
}


//---------------------------------------------------------------------------------------------------------------------
// Destructor
//---------------------------------------------------------------------------------------------------------------------
Process::~Process(void)
{
	if (m_pChild)
	{
		m_pChild->VOnAbort();
	}
}





StrongProcessPtr Process::RemoveChild(void)
{
	if (m_pChild)
	{
        StrongProcessPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
		m_pChild.reset();
		
        return pChild;
	}

	return StrongProcessPtr();
}


