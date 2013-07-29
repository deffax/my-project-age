#include "GameCodeStd.h"
#include "Process.h"

Process::Process()
{
	m_state = UNINITIALIZED;
}

Process::~Process()
{
	if(m_pChild)
		m_pChild->VOnAbort();
}

StrongProcessPtr Process::RemoveChild()
{
	if(m_pChild)
	{
		StrongProcessPtr pChild = m_pChild;
		m_pChild.reset();
		return m_pChild;
	}

	return StrongProcessPtr();
}