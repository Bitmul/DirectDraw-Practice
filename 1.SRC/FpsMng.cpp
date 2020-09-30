#include <windows.h>
#include <mmsystem.h>
#include "FpsMng.h"
#include "DxMng.h"

CFpsMng::CFpsMng()
{
    m_dwFps = 0;

	m_dwFpsMin=999;
	m_dwFpsMax=0;
	m_dwFpsSum=0;
	m_dwFpsAvr=0;	

	SetFPS(0);
}

CFpsMng::~CFpsMng()
{
}

void CFpsMng::SetFPS(DWORD dwFPS)
{
	///// �����ð� ����
	if (dwFPS == 0)
	{
		m_dwFPM = 0;
	}
	else ///// �Ϲ�
	{
		m_dwFPM = 1000 / dwFPS;
		m_dwOldMSec = timeGetTime();
	}

	m_dwCount = 1;
}

BOOL CFpsMng::IsRender()
{
    DWORD dwCurTime = timeGetTime();
    if (dwCurTime - m_dwOldSkipTime <= m_dwFPM)
    {
        m_dwOldSkipTime = dwCurTime;
        return TRUE;
    }

    m_dwOldSkipTime = dwCurTime;
    return FALSE;
}

DWORD CFpsMng::GetFPS()
{
	m_dwFpsTime = timeGetTime() - m_dwLastTime;

	m_dwFramesRendered++;

	if (m_dwFpsTime > 1000)
	{
		m_dwFps = m_dwFramesRendered;
		m_dwFramesRendered  = 0;
		m_dwFpsTime = 0;
		m_dwLastTime  = timeGetTime();
	}

    return m_dwFps;
}

void CFpsMng::GetFPSInfo(RECT* pInfo)
{
	m_dwFpsTime = timeGetTime() - m_dwLastTime;

	m_dwFramesRendered++;

	if (m_dwFpsTime > 1000)
	{
		m_dwFps = m_dwFramesRendered;
		m_dwFramesRendered  = 0;
		m_dwFpsTime = 0;

		///// �ΰ����� ( ��Ȯ���� ���� 2ȸ ���� üũ )		
		if (m_dwCount > 1)
		{
			if (m_dwFps < m_dwFpsMin) m_dwFpsMin = m_dwFps;
			if (m_dwFps > m_dwFpsMax) m_dwFpsMax = m_dwFps;		
			m_dwFpsSum += m_dwFps;
			m_dwFpsAvr = m_dwFpsSum / (m_dwCount-1);
		}

		m_dwCount++;

		m_dwLastTime  = timeGetTime();
	}
    
	::SetRect(pInfo, m_dwFps, m_dwFpsMin, m_dwFpsMax, m_dwFpsAvr);	
}

DWORD CFpsMng::GetProcTime()
{
	return m_dwProcTime;
}

void CFpsMng::FrameTuning()
{
	DWORD dwFPM = m_dwFPM;
	DWORD dwOldMSec = m_dwOldMSec;

	::timeBeginPeriod(1);
	{
		///// 1 frame ó���ð� ( ������, ���, ��Ÿ )		
		m_dwProcTime = timeGetTime() - dwOldMSec;

		/////
		while (timeGetTime() - dwOldMSec < m_dwFPM);

		/////
		m_dwOldMSec = timeGetTime();
	}
	::timeEndPeriod(1);
}

void CFpsMng::FrameTuningEx()
{
	///// 1 frame ó���ð� ( ������, ���, ��Ÿ )			
	m_dwProcTime = timeGetTime() - m_dwOldMSec;	

	if (m_dwFPM)
	{
		///// FPM�� �����Ѵ�
		DWORD dwRevision = 0;
		if (m_dwFPM > m_dwProcTime) dwRevision = m_dwFPM - m_dwProcTime;

		///// Ÿ�̸��� ���е��� 1ms
		::timeBeginPeriod(1);
		{
			::Sleep(dwRevision);
		}
		::timeEndPeriod(1);
	}
	/////	
	m_dwOldMSec = timeGetTime();	
}