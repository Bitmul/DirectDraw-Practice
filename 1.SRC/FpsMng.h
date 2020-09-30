#ifndef __FPSMNG_H__
#define __FPSMNG_H__

class CFpsMng
{
public:
    CFpsMng();						// 디폴트는 지연시간 없음
    virtual ~CFpsMng();


    void  SetFPS(DWORD dwFPS=0);	// dwFPS == 0, 지연시간 없음
	DWORD GetFPS(void);				// 현재 FPS를 얻는다
	void  GetFPSInfo(RECT* pInfo);	// 현재 FPS와 부가정보를 리턴한다
	DWORD GetProcTime(void);		// 1 Frame 처리시간 리턴
    
    void  FrameTuning(void);		// 설정한 프레임수로 조절 ( Polling  )
	void  FrameTuningEx(void);		// 설정한 프레임수로 조절 ( Blocking )
	
	BOOL  IsRender(void);			// 지연시간 초과여부

private:
	DWORD m_dwLastTime;	
	DWORD m_dwFpsTime;	
	DWORD m_dwFramesRendered;
	DWORD m_dwFps;

    DWORD m_dwFPM;               // Frame Per Millisecond
    DWORD m_dwOldMSec;           // Old Millisecond
    DWORD m_dwOldSkipTime;

	///// 부가정보 변수
	DWORD m_dwFpsMin;			 // 최소 FPS
	DWORD m_dwFpsMax;			 // 최대 FPS
	DWORD m_dwFpsSum;			 // 합계 FPS ( 누적 )
	DWORD m_dwFpsAvr;			 // 평균 FPS ( 누적 )
	DWORD m_dwCount;             // 평균 FPS ( 카운트 )
	DWORD m_dwProcTime;			 // 1 Frame 처리시간

};

#endif