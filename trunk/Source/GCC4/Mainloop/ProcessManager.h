#pragma once



#include "Process.h"

class ProcessManager
{
	typedef std::list<StrongProcessPtr> ProcessList;

	ProcessList m_processList;

public:
	
	
	~ProcessManager(void);

	
	unsigned int UpdateProcesses(unsigned long deltaMs);  // updates all attached processes
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);  // attaches a process to the process mgr
	void AbortAllProcesses(bool immediate);

	
	unsigned int GetProcessCount(void) const { return m_processList.size(); }

private:
	void ClearAllProcesses(void);  
};


