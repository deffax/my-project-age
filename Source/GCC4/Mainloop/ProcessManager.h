#pragma once
#include "Process.h"

class ProcessManager
{
	typedef std::list<StrongProcessPtr> ProcessList;
	ProcessList m_processList;

public:
	~ProcessManager();

	unsigned int UpdateProcess(unsigned long int deltaMs);
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);
	void AbortAllProcess(bool immediate);

	unsigned int GetProcessCount() const {return m_processList.size();}

private:
	void ClearAllProcesses();
};