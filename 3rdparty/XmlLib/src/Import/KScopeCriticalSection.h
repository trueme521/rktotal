//////////////////////////////////////////////////////////////////////
///		@file		KScopeCriticalSection.h
///		@author		luopeng
///		@date		2007-5-21 11:51:52
///
///		@brief		
//////////////////////////////////////////////////////////////////////

#ifndef KSCOPECRITICALSECTION_H
#define KSCOPECRITICALSECTION_H

class KScopeCriticalSection
{
public:
	KScopeCriticalSection(CRITICAL_SECTION& cs) : m_cs(cs)
	{
		EnterCriticalSection(&m_cs);
	}

	~KScopeCriticalSection()
	{
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION& m_cs;
};

#endif  // KSCOPECRITICALSECTION_H
