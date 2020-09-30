#ifndef __FPSMNG_H__
#define __FPSMNG_H__

class CFpsMng
{
public:
    CFpsMng();						// ����Ʈ�� �����ð� ����
    virtual ~CFpsMng();


    void  SetFPS(DWORD dwFPS=0);	// dwFPS == 0, �����ð� ����
	DWORD GetFPS(void);				// ���� FPS�� ��´�
	void  GetFPSInfo(RECT* pInfo);	// ���� FPS�� �ΰ������� �����Ѵ�
	DWORD GetProcTime(void);		// 1 Frame ó���ð� ����
    
    void  FrameTuning(void);		// ������ �����Ӽ��� ���� ( Polling  )
	void  FrameTuningEx(void);		// ������ �����Ӽ��� ���� ( Blocking )
	
	BOOL  IsRender(void);			// �����ð� �ʰ�����

private:
	DWORD m_dwLastTime;	
	DWORD m_dwFpsTime;	
	DWORD m_dwFramesRendered;
	DWORD m_dwFps;

    DWORD m_dwFPM;               // Frame Per Millisecond
    DWORD m_dwOldMSec;           // Old Millisecond
    DWORD m_dwOldSkipTime;

	///// �ΰ����� ����
	DWORD m_dwFpsMin;			 // �ּ� FPS
	DWORD m_dwFpsMax;			 // �ִ� FPS
	DWORD m_dwFpsSum;			 // �հ� FPS ( ���� )
	DWORD m_dwFpsAvr;			 // ��� FPS ( ���� )
	DWORD m_dwCount;             // ��� FPS ( ī��Ʈ )
	DWORD m_dwProcTime;			 // 1 Frame ó���ð�

};

#endif