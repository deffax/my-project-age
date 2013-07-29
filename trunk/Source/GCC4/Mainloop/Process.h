#pragma once

class Process;
typedef shared_ptr<Process> StrongProcessPtr;
typedef weak_ptr<Process> WeakProcessPtr;

class Process
{
	friend class ProcessManager;

public:
	enum State
	{
		UNINITIALIZED = 0,
		REMOVED,
		RUNNING,
		PAUSED,
		SUCCEEDED,
		FAILED,
		ABORTED
	};

private:
	State m_state;
	StrongProcessPtr m_pChild;

public:
	Process();
	virtual ~Process();

protected:
	virtual void VOnInit() {m_state = RUNNING;}
	virtual void VOnUpdate(unsigned long deltaMs) = 0;
	virtual void VOnSuccess() {}
	virtual void VOnFail() {}
	virtual void VOnAbort() {}

public:
	inline void Pause();
	inline void UnPause();

	inline void Succeeded();
	inline void Fail();

	State GetState() const {return m_state;}
	bool IsAlive() const {return (m_state == RUNNING || m_state == PAUSED);}
	bool IsDead() const {return (m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED);}
	bool IsRemoved() const {return (m_state == REMOVED);}
	bool IsPaused() const {return (m_state == PAUSED);}

	inline void AttachChild(StrongProcessPtr pChild);
	StrongProcessPtr RemoveChild();
	StrongProcessPtr PeekChild() const {return m_pChild;}

private:
	void SetState(State newState) {m_state = newState;}
};


inline void Process::Succeeded()
{
	GCC_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_state = SUCCEEDED;
}

inline void Process::Fail()
{
	GCC_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_state = FAILED;
}

inline void Process::AttachChild(StrongProcessPtr pChild)
{
	if(m_pChild)
		m_pChild->AttachChild(pChild);
	else
		m_pChild = pChild;
}

inline void Process::Pause()
{
	if(m_state == RUNNING)
		m_state = PAUSED;
	else
		GCC_WARNING("Attemping to pause a process that isn't running");
}

inline void Process::UnPause()
{
	if(m_state == PAUSED)
		m_state = RUNNING;
	else
		GCC_WARNING("Attemping to pause a process that isn't paused");
}