#include "DxWFrame.h"

#include "ddutil.h"
#include "FpsMng.h"
#include "HHsound.h"

class _DDSET_ {};

#define HRES  1024
#define VRES  768
#define BPP    16 // Bit Per Pixel
#define REF	    0  // Refresh Rate

#define BBC     1 // Back Buff Count
#define BOXCOUNT 10

#define _FPS   60
#define _TT_MOVE
#define _DISPLAYINFO
#define _TT_INPUT
#define _NOVSYNC 0

//// 폰트 및 윈도우 설정
#define FNTSZ  16
#define FNTWH  20

#define HEDGE   6
#define VEDGE  25

#define PEN WHITE

#define RGB565 565
#define RGB555 555
#define BGR565 -565
#define RGB888 888
#define BGR888 -888

/////
APP_SET     gAppSet;
BOOL        gbIsGod = FALSE;
BOOL		gbIsFirst = TRUE;
BOOL        gbIsFull = TRUE;

HFONT		gFont;
HDC			gHdc;

CFpsMng		gFpsMng;

POINT		g_Pos,g_BPos; //플레이어의 위치,보스의 위치 || 참고로 Point 자료형은 x와 y좌표를 가지고있는 구조체입니다.
BOOL		g_bFadeIn = FALSE, g_bFadeOut = FALSE, g_bFadeLoop = FALSE;
BOOL		g_bInc = FALSE;
BOOL        g_bJump = FALSE, g_bAccel = FALSE, g_bJaccel = FALSE, g_bRight = FALSE, g_bLeft = FALSE;
BOOL        g_bJRight = FALSE, g_bJLeft = FALSE, g_bSetSpeed = FALSE;
BOOL		g_bGrayScale = FALSE, g_bColorize = FALSE;
BOOL        g_bLookR = TRUE, g_bLookL = FALSE;
BOOL        g_bBLR = TRUE, g_bBLL = FALSE;
BOOL        g_bRush = FALSE;
BOOL        g_bAR = FALSE, g_bAL = FALSE, g_bAtt = FALSE;
BOOL        g_bRushR, g_bRushL = FALSE;
BOOL        musicstart = FALSE;
BOOL        bossimmu = FALSE;
BOOL        g_bBAT = FALSE;
BOOL        Delay = FALSE;
DWORD		g_dwFadeRate = 0;
DWORD       g_dwR, g_dwG, g_dwB;

//g_bJump = 점프를 했는가?
//g_bAccel = 가속중인가?
//g_bJaccel = 점프 가속중인가?
//g_bRight | g_bLeft = 오른쪽 또는 왼쪽으로 이동중인가?
//g_bJRight | g_bJLeft = 오른쪽 또는 왼쪽으로 점프중인가?
//g_bSetSpeed = 가속도 한계치가 설정 됐는가?
//g_bLookR | g_bLookL = 오른쪽 또는 왼쪽을 보고있는가?
//g_BLR | g_BLL = 보스가 왼쪽 또는 오른쪽을 보고있는가?
//g_bAR | g_bAL = 오른쪽 또는 왼쪽으로 공격중인가?
//g_bAtt = 공격중인가?
DWORD g_dwFADELUT[64][101];

DDSURFACEDESC2 g_ddsd;
DWORD g_dwPixelFormat;
DWORD g_dwRGBBitCount;

CHHSound m_HHSound;
LPDIRECTSOUNDBUFFER g_pDS[7];
//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
LPDIRECTDRAW7               g_pDD = NULL;        // DirectDraw object
LPDIRECTDRAWSURFACE7        g_pDDSPrimary = NULL;// DirectDraw primary surface
LPDIRECTDRAWSURFACE7        g_pDDSBack = NULL;   // DirectDraw back surface
LPDIRECTDRAWSURFACE7        g_pDDSOne = NULL;    // PlayerL Surface
LPDIRECTDRAWSURFACE7        g_pDDSOneR = NULL;    // PlayerR Surface
LPDIRECTDRAWSURFACE7        g_pDDSTwo = NULL;    // BossL Surface
LPDIRECTDRAWSURFACE7        g_pDDSTwoR = NULL;    // BossR Surface
LPDIRECTDRAWSURFACE7        g_pDDSSkill = NULL;    // SkillL Surface
LPDIRECTDRAWSURFACE7        g_pDDSSkillR = NULL;    // SkillR Surface
LPDIRECTDRAWSURFACE7        g_pDDSHP = NULL;    // HP Surface
LPDIRECTDRAWSURFACE7        g_pDDSTITLE = NULL;    // TITLE Surface
LPDIRECTDRAWSURFACE7        g_BossAtt = NULL;   //BossAtt Surface
LPDIRECTDRAWSURFACE7        g_BossAtt2 = NULL;   //BossAtt2 Surface
LPDIRECTDRAWSURFACE7        g_pDDSHPbar = NULL;    // HPbar Surface
LPDIRECTDRAWSURFACE7        g_pDDSPHP = NULL;    // Player HP Surface
LPDIRECTDRAWSURFACE7        g_pDDSCHARGE = NULL;    // Charge Surface
LPDIRECTDRAWSURFACE7        g_pDDSCHARGER = NULL;    // ChargeRight Surface
LPDIRECTDRAWSURFACE7        g_pDDSPHPbar = NULL;    // Player HPbar Surface
LPDIRECTDRAWSURFACE7        g_pDDSBG = NULL;    // BackGround Surface 1
LPDIRECTDRAWSURFACE7        g_pDDSBG2 = NULL;    // BackGround Surface 2
LPDIRECTDRAWSURFACE7        g_pDDSBG3 = NULL;    // BackGround Surface 3
LPDIRECTDRAWSURFACE7        g_pDDSCast = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSCast2 = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSCastAtt = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBossAtt = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBossAttR = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBoss3Att = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBoss3AttR = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBoss3Att2 = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBoss3AttR2 = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBoss3Att3 = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSBoss3AttR3 = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSGroundAtt = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSDragAtt = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSDragAttR = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSP3standL = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSDHole = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSP3standR = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSDBossHit = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSDPlayerHit = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSEnd = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSGameover = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSPlayerskill = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSSkillBut = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSSkillCool = NULL;
LPDIRECTDRAWPALETTE         g_pDDPal = NULL;     // The primary surface palette

LPDIRECTDRAWCLIPPER         g_pDDClipper = NULL;

LPDIRECTDRAWSURFACE7        g_pDDSVBuffer = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSSBuffer = NULL;
LPDIRECTDRAWSURFACE7        g_pDDSSBuffer2 = NULL;
int Bx = 0;
int Gravity = 13; //중력 설정
int Hspeed = 0; //가속도 설정
int P1MHP = 3000; //페이즈 1 체력
int P1HP = P1MHP;
int P2MHP = 5000;
int P2HP = P2MHP;
int P3MHP = 10000;
int P3HP = P3MHP;
int cx, cy = 0; //발사체 좌표
int o = 0;
int P3randpat = 0;
int P3randpat2 = 0;
BOOL isSafe = FALSE;
BOOL g_bCast = FALSE;
BOOL g_bCRight = FALSE;
BOOL g_bCLeft = FALSE;
BOOL g_bCFire = FALSE;
BOOL g_bHit = FALSE;
BOOL Mirror = FALSE; //공격 반전
BOOL BossAtt = FALSE;
BOOL BAR, BAL = FALSE;
BOOL BARR, BALL = FALSE;
BOOL Muffle = FALSE;
BOOL P5Mus = FALSE;
BOOL CastHit = FALSE;
BOOL Skilluse = FALSE;
BOOL BigHit = FALSE; //필살기 막타
int PlayerMHP = 40000;
int PlayerHP = PlayerMHP;
int Randpat = 0;
int Randpat2 = 0;
BOOL P2musicstart = 0;
int RandNum[5] = { 0, };
int Maximum = 0;
int Knockspd = 0;
float PlayerHPrate = 1;
float P1HPrate = 1;
float P2HPrate = 1;
float P3HPrate = 1;
int KnockHspeed = 0; //넉백가속도
int Bframe = 0; //보스 애니 프레임
int BAframe = 0; //보스 공격애니 프레임
int Aframe = 0; //공격 애니 프레임
int Cframe = 0; //돌격 애니 프레임
int CAframe = 0; //마법진 애니 프레임
int CAAframe = 0; //발사체 애니 프레임
int BKframe = 0; //보스 넉백공격 애니 프레임
int DRframe = 0;//끌기 애니 프레임
int GAframe = 0;//땅찍기 애니 프레임
int SPframe = 0;//가시 애니 프레임
int PHframe = 0;//플레이어 피격애니 프레임
int BHframe[3] = { 0 };//보스 피격애니 프레임
int PSframe = 0; //플레이어 필살기 애니 프레임
int SpikeX[10] = { 0 };
int SpikeY = 0;
int MaxTime = 500; //쿨타임 시간
int Skillcheat = 0; //필살기 댐증
float Timerrate = 1;
BOOL Pimmune = FALSE;
RECT HIT,BOSS,rcDest,BAHIT[5],PHIT,Cast,Knock,Spike[10];
BOOL p1die = FALSE; //페이즈 1 보스가 죽었는가?
BOOL p2die = FALSE;
BOOL p3die = FALSE;
BOOL Drag = FALSE;
BOOL Knockback = FALSE; // 넉백중
BOOL KnockHIT = FALSE; //넉백 한대 맞음
BOOL p3musicstart = FALSE;
BOOL P4Mus = FALSE;
BOOL g_bGatt = FALSE; //땅찍기 공격중인가?
BOOL g_bSpike = FALSE; //바닥 가시 출현중인가?
BOOL PHplay = FALSE; //플레이어/보스 피격 애니가 재생중인가?
BOOL BHplay[3] = { FALSE,};
BOOL BHMPlay = FALSE; //보스 피격애니중?
int g_nPhaze = 0; //페이즈를 1 페이즈로 설정
int Timer[32] = { 0,}; //0번째 타이머 - 캐릭터 이동 2번째 - 보스 이동 3번째 - 공격 애니메이션 4번째 - 플레이어 공격 5번째 - 보스 피격무적 6번째 - 공격 애니
//7번째 - 거미다리 공격 쿨타임 8번째 - 플레이어 돌진기 9번째 - 돌진기 쿨타임 10번째 - 캐스팅 애니메이션 11번째 - 캐스팅 공격 애니 12번째 - 랜덤 캐스팅 시간
//13번째- 2페이즈 캐스팅 14번째 - 공격 반전 시간 15번째 - 공격 반전 쿨타임 16번째 - 보스 넉백 애니 17번째 - 보스 넉백 쿨타임
//18번째 - 끌기 공격 애니 19번째 - 끌기 공격 쿨타임 20번째 - 보스 바닥찍기 공격 쿨타임 21번째 - 보스 바닥찍기 공격 애니 22번째 - 바닥가시 출현 쿨타임 23번째 - 바닥가시 출현 애니
//24번째 - 보스 피격 애니 25번째 - 플레이어 피격 애니 26번째 - 보스 피격 애니 2 27번째 보스 피격 애니 3 28번째 필살기 쿨타임 29번째 필살기 애니
BOOL _ChangeDisplayMode(DWORD dwWidth, DWORD dwHeight, WORD wBPP)
{
	DEVMODE dm;
	LONG    rval;
	
	dm.dmPelsWidth  = dwWidth;
	dm.dmPelsHeight = dwHeight;
	dm.dmBitsPerPel = wBPP;
	dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    rval = ChangeDisplaySettings(&dm, 0);
    if (rval != DISP_CHANGE_SUCCESSFUL) return FALSE;

	return TRUE;
}

BOOL __Init(HINSTANCE hInstance, int nCmdShow )
{
	/////	
	gAppSet.bIsActive      = FALSE;
	gAppSet.bIsDisplayInfo = FALSE;
	gAppSet.bIsPause       = FALSE;	
	gAppSet.bIsRenderBlank = FALSE;

    /////
    WNDCLASS  wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = __WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = NULL;//GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NAME;
    wc.lpszClassName = NAME;
    RegisterClass( &wc );

    DWORD ExWinStyle, WinStyle;

    #ifdef _DEBUG
        ExWinStyle = NULL;
        WinStyle = WS_POPUPWINDOW | WS_CAPTION;
    #else
        ExWinStyle = WS_EX_TOPMOST;
        WinStyle = WS_POPUP;
    #endif

    hWnd = CreateWindowEx(ExWinStyle,
                        NAME,
                        TITLE,
                        WinStyle,
                        0,
                        0,
                        HRES + HEDGE,
                        VRES + VEDGE,
                        NULL,
                        NULL,
                        hInstance,
                    	NULL);

    if( !hWnd )	return FALSE;

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    /////
    HANDLE hProc = ::GetCurrentProcess();
    #ifdef _DEBUG
		::SetPriorityClass (hProc, NORMAL_PRIORITY_CLASS);
    #else
		::SetPriorityClass (hProc, HIGH_PRIORITY_CLASS);
    #endif
		
	   m_HHSound.Init(hWnd);

		//사운드 초기화
		m_HHSound.LoadWave("music/Diffraction.wav", &g_pDS[0]);
		m_HHSound.SetVolume(g_pDS[0], 100);

		m_HHSound.LoadWave("music/MirrorCage.wav", &g_pDS[1]);
		m_HHSound.SetVolume(g_pDS[1], 100);

		m_HHSound.LoadWave("music/BloodCage.wav", &g_pDS[2]);
		m_HHSound.SetVolume(g_pDS[2], 100);

		m_HHSound.LoadWave("sound/RagingBlow.wav", &g_pDS[3]);
		m_HHSound.SetVolume(g_pDS[3], 100);

		m_HHSound.LoadWave("sound/Charge.wav", &g_pDS[4]);
		m_HHSound.SetVolume(g_pDS[4], 100);

		m_HHSound.LoadWave("sound/Worldreaver.wav", &g_pDS[5]);
		m_HHSound.SetVolume(g_pDS[5], 100);

		m_HHSound.LoadWave("sound/WorldreaverHit.wav", &g_pDS[6]);
		m_HHSound.SetVolume(g_pDS[6], 100);
		
		///// 입력장치 초기화
	///// 입력장치 초기화


	/////
    BOOL rval = DXW_Init();
    if (!rval) return FALSE;

    return TRUE;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    MSG		msg;
    
	//// 창모드 설정

	////
    if(!__Init(hInstance, nCmdShow)) return FALSE;

	TRACE1("===== Open Process ===== [ DxWFrame - %x]\n", ::GetCurrentProcessId());
	TRACE3("HRes %4d VRes %4d Bpp %2d\n", HRES, VRES, BPP);

    while(1)
    {
    	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    	{
    	    if( !GetMessage( &msg, NULL, 0, 0 ) ) break;			
    	    TranslateMessage(&msg);
    	    DispatchMessage(&msg);
    	}
    	else if( gAppSet.bIsActive && !gAppSet.bIsPause)
		//else
    	{
			DXW_MainLoop();	
        }
        else WaitMessage();		
    }

	TRACE1("===== Close Process ===== [ DxWFrame - %x]\n", ::GetCurrentProcessId());

    return msg.wParam ;
}

LRESULT CALLBACK __WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch( message )
    {
    case WM_KEYDOWN     : __WinKeyDown(hwnd, wParam); break;
    case WM_CHAR        :
        if (wParam == 'i' || wParam == 'I') { gAppSet.bIsDisplayInfo = !gAppSet.bIsDisplayInfo; break; }
		if (wParam == 'p' || wParam == 'P')
		{
			gAppSet.bIsPause = !gAppSet.bIsPause;			
			break;
		}
		break;
    case WM_ACTIVATEAPP : gAppSet.bIsActive = wParam;         break;
	case WM_MOUSEMOVE :
		//g_MousePos = MAKEPOINTS(lParam);
		break;
	case WM_QUERYNEWPALETTE: break;	
    case WM_DESTROY:
        __WinDestroy();
		PostQuitMessage(0);						
		break;	
    case WM_SETCURSOR:
		if (gbIsFull)
		{
			SetCursor(NULL);	
			return TRUE;
		}		
		break;
	}

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void __WinKeyDown(HWND hwnd, WPARAM wParam)
{
    static BOOL bIsFpsMax = FALSE;

	switch( wParam )
	{
	case VK_ESCAPE:
		PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;

	case VK_F1:
		Skillcheat += 100;
		break;
	case VK_F2:
		Skillcheat -= 100;
		break;
	case VK_F3:
		
		break;

    case VK_F4:	
		g_nPhaze = 4;
        break;

    case VK_F5: __FxFadeTo(g_pDDSPrimary, g_pDDSBack, rand() % 32, rand() % 64, rand() % 32); break;
	case VK_F6: g_bGrayScale = !g_bGrayScale; break;		
	case VK_F7: g_bColorize = !g_bColorize;
				g_dwR = rand() % 32;
				g_dwG = rand() % 64;
				g_dwB = rand() % 32;
				break;		
	case VK_F8: gbIsGod = !gbIsGod; break;		
    case VK_F9: 
		bIsFpsMax = !bIsFpsMax;
		if (bIsFpsMax) 
			gFpsMng.SetFPS(0);
		else 
			gFpsMng.SetFPS(_FPS);
        break;
	case VK_F10: gFpsMng.SetFPS(30); break;

    case VK_TAB :
        break;

    case VK_BACK :
        break;

    case VK_END : 
        break;

    case VK_HOME :
        break;

    case VK_DELETE :
        break;

	case VK_SPACE :
		break;

    case VK_RETURN:
		break;
	}
}

void __WinDestroy()
{
	//// 디버그 모드를 위한 비트픽셀 복구

	////
	DXW_ReleaseWinMode();
	
	////
	__DestroyFont();

	//// DX 오브젝트 해제
	__ReleaseAllObjects();

	////
	_CrtDumpMemoryLeaks();

    TRACE0("__WinDestroy\n");  
}

void DXW_ReleaseWinMode()
{
    TRACE0("DXW_ReleaseWinMode\n");
}

void DXW_LUTInit()
{	
	//// Fade Look Up Table 생성
	for (DWORD dwDepth=0; dwDepth < 64; dwDepth++)
	{
		for (DWORD dwRate=0; dwRate <= 100; dwRate++)
		{
			g_dwFADELUT[dwDepth][dwRate] = dwDepth * dwRate/100;
		}
	}	
}

BOOL DXW_Init()
{
    ///// DX 객체 생성 및 초기화
	BOOL bRval;

	///// 
#ifdef _DEBUG
	bRval = __DDInitWin(TRUE);
	if (!bRval) 
	{		
		bRval = __DDInit();
		if (!bRval) return FALSE;
	}
	//bRval = __DDCSetClipper(g_pDDSBack, 0, 0, HRES, VRES);
	bRval = __DDCSetClipperWin();
	if (!bRval) return FALSE;
#else
	bRval = __DDInit();
	if (!bRval) return FALSE;
	bRval = __DDCSetClipper(g_pDDSBack, 0, 0, HRES, VRES);
	if (!bRval) return FALSE;
#endif
	
	///// 픽셀포맷 체크
	g_dwPixelFormat = ::__CheckRGBBit(g_pDDSPrimary);

	///// 데이터 불러오기
	/*switch (g_dwPixelFormat)
	{
	case RGB565 : g_pDDSOne = __DDLoadBin("man16.bin", TRUE); break;				  
	case RGB888 : g_pDDSOne = __DDLoadBin("man32.bin", TRUE); break;
	default : return FALSE;
	}*/
	g_pDDSOne = ::DDLoadBitmap(g_pDD, "image/Hero.bmp", 774, 81, TRUE);
	::DDSetColorKey(g_pDDSOne, RGB(181, 230, 29));
	g_pDDSDHole = ::DDLoadBitmap(g_pDD, "image/Hole.bmp", 87, 36, TRUE);
	::DDSetColorKey(g_pDDSDHole, RGB(0, 255, 0));
	g_pDDSSkillBut = ::DDLoadBitmap(g_pDD, "image/SkillButton.bmp", 64, 34, TRUE);
	::DDSetColorKey(g_pDDSSkillBut, RGB(0, 255, 0));
	g_pDDSSkillCool = ::DDLoadBitmap(g_pDD, "image/SkillCool.bmp", 62, 32, TRUE);
	::DDSetColorKey(g_pDDSSkillCool, RGB(0, 255, 0));
	g_pDDSCast = ::DDLoadBitmap(g_pDD, "image/Cast3.bmp", 3840,192, TRUE); //애니메이션 20개.
	::DDSetColorKey(g_pDDSCast, RGB(0, 255, 0));
	g_pDDSCast2 = ::DDLoadBitmap(g_pDD, "image/Cast4.bmp", 3840, 192, TRUE); //애니메이션 20개.
	::DDSetColorKey(g_pDDSCast2, RGB(255, 0, 255));
	g_pDDSCastAtt = ::DDLoadBitmap(g_pDD, "image/CastAttack.bmp", 3440, 525, TRUE); //애니메이션 10개.
	::DDSetColorKey(g_pDDSCastAtt, RGB(0,255,0));
	g_pDDSBossAtt = ::DDLoadBitmap(g_pDD, "image/BossAttackMotionL.bmp", 7116, 321, TRUE); //애니메이션 12개.
	::DDSetColorKey(g_pDDSBossAtt, RGB(0, 255, 0));
	g_pDDSBossAttR = ::DDLoadBitmap(g_pDD, "image/BossAttackMotionR.bmp", 7116, 321, TRUE); //애니메이션 12개.
	::DDSetColorKey(g_pDDSBossAttR, RGB(0, 255, 0));
	g_pDDSDragAttR = ::DDLoadBitmap(g_pDD, "image/BossAttack2R.bmp", 12240, 315, TRUE); //애니메이션 17개.
	::DDSetColorKey(g_pDDSDragAttR, RGB(0, 255, 0));
	g_pDDSDragAtt = ::DDLoadBitmap(g_pDD, "image/BossAttack2L.bmp", 12240, 315, TRUE); //애니메이션 17개.
	::DDSetColorKey(g_pDDSDragAtt, RGB(0, 255, 0));
	g_pDDSPlayerskill = ::DDLoadBitmap(g_pDD, "image/Worldreaver.bmp", 20480, 768, TRUE); //애니메이션 20개.
	::DDSetColorKey(g_pDDSPlayerskill, RGB(0, 255, 0));
	g_pDDSBoss3Att = ::DDLoadBitmap(g_pDD, "image/Boss3AttackL.bmp", 16100, 384, TRUE); //애니메이션 24개.
	::DDSetColorKey(g_pDDSBoss3Att, RGB(0, 255, 0));
	g_pDDSBoss3AttR = ::DDLoadBitmap(g_pDD, "image/Boss3AttackR.bmp", 16100, 384, TRUE); //애니메이션 24개.
	::DDSetColorKey(g_pDDSBoss3AttR, RGB(0, 255, 0));
	g_pDDSBoss3Att3 = ::DDLoadBitmap(g_pDD, "image/Boss3Attack33L.bmp", 35247, 798, TRUE); //애니메이션 31개.
	::DDSetColorKey(g_pDDSBoss3Att3, RGB(0, 255, 0));
	g_pDDSBoss3AttR3 = ::DDLoadBitmap(g_pDD, "image/Boss3Attack33R.bmp", 35247, 798, TRUE); //애니메이션 31개.
	::DDSetColorKey(g_pDDSBoss3AttR3, RGB(0, 255, 0));
	g_pDDSGroundAtt= ::DDLoadBitmap(g_pDD, "image/GroundAttack.bmp", 6330, 629, TRUE); //애니메이션 15개.
	::DDSetColorKey(g_pDDSGroundAtt, RGB(0, 0, 255));
	g_pDDSOneR = ::DDLoadBitmap(g_pDD, "image/HeroRR.bmp", 774, 81, TRUE);
	::DDSetColorKey(g_pDDSOneR, RGB(181, 230, 29));
	g_BossAtt = ::DDLoadBitmap(g_pDD, "image/BossAtt2.bmp", 12000, 656, TRUE);
	::DDSetColorKey(g_BossAtt, RGB(255, 255, 255));
	g_BossAtt2 = ::DDLoadBitmap(g_pDD, "image/BossAtt3.bmp", 12000, 656, TRUE);
	::DDSetColorKey(g_BossAtt2, RGB(0, 0, 0));
	g_pDDSTwo = ::DDLoadBitmap(g_pDD, "image/BossStand.bmp", 1392, 148, TRUE);
	::DDSetColorKey(g_pDDSTwo, RGB(34, 177, 76));
	g_pDDSTwoR = ::DDLoadBitmap(g_pDD, "image/BossStandR.bmp", 1392, 148, TRUE);
	::DDSetColorKey(g_pDDSTwoR, RGB(34, 177, 76));
	g_pDDSHP = ::DDLoadBitmap(g_pDD, "image/BossHP.bmp", 762, 16, TRUE);
	::DDSetColorKey(g_pDDSHP, RGB(34, 177, 76));
	g_pDDSCHARGE = ::DDLoadBitmap(g_pDD, "image/Charge.bmp", 4088, 263, TRUE);
	::DDSetColorKey(g_pDDSCHARGE, RGB(181, 230, 29));
	g_pDDSCHARGER = ::DDLoadBitmap(g_pDD, "image/ChargeR.bmp", 4088, 263, TRUE);
	::DDSetColorKey(g_pDDSCHARGER, RGB(181, 230, 29));
	g_pDDSHPbar = ::DDLoadBitmap(g_pDD, "image/BossHPbar.bmp", 758, 12, TRUE);
	::DDSetColorKey(g_pDDSHPbar, RGB(34, 177, 76));
	g_pDDSPHP = ::DDLoadBitmap(g_pDD, "image/PlayerHP.bmp", 202, 29, TRUE);
	::DDSetColorKey(g_pDDSPHP, RGB(34, 177, 76));
	g_pDDSPHPbar = ::DDLoadBitmap(g_pDD, "image/PlayerHPbar.bmp", 169, 11, TRUE);
	::DDSetColorKey(g_pDDSPHPbar, RGB(34, 177, 76));
	g_pDDSSkill = ::DDLoadBitmap(g_pDD, "image/RagingBlow.bmp", 3913, 218, TRUE);
	::DDSetColorKey(g_pDDSSkill, RGB(34, 177, 76));
	g_pDDSSkillR = ::DDLoadBitmap(g_pDD, "image/RagingBlowR.bmp", 3913, 218, TRUE);
	::DDSetColorKey(g_pDDSSkillR, RGB(34, 177, 76));
	g_pDDSP3standL = ::DDLoadBitmap(g_pDD, "image/Boss3StandL.bmp", 3888, 314, TRUE);
	::DDSetColorKey(g_pDDSP3standL, RGB(0, 255, 0));
	g_pDDSP3standR = ::DDLoadBitmap(g_pDD, "image/Boss3StandR.bmp", 3888, 314, TRUE);
	::DDSetColorKey(g_pDDSP3standR, RGB(0, 255, 0));
	g_pDDSDBossHit = ::DDLoadBitmap(g_pDD, "image/BossHit.bmp", 1080, 226, TRUE);
	::DDSetColorKey(g_pDDSDBossHit, RGB(0, 255, 0));
	g_pDDSDPlayerHit = ::DDLoadBitmap(g_pDD, "image/PlayerHit.bmp", 1080, 226, TRUE);
	::DDSetColorKey(g_pDDSDPlayerHit, RGB(0, 255, 0));
	if (!g_pDDSOne) return MSGERROR("오프스크린생성실패");
	g_pDDSTITLE = ::DDLoadBitmap(g_pDD, "image/Title.bmp", HRES, VRES, TRUE);
	g_pDDSBG = ::DDLoadBitmap(g_pDD, "image/MI1.bmp", HRES, VRES, TRUE);
	g_pDDSBG2 = ::DDLoadBitmap(g_pDD, "image/MI2.bmp", HRES, VRES, TRUE);
	g_pDDSBG3 = ::DDLoadBitmap(g_pDD, "image/MI3.bmp", HRES, VRES, TRUE);
	g_pDDSGameover = ::DDLoadBitmap(g_pDD, "image/GameOver.bmp", HRES, VRES, TRUE);
	g_pDDSEnd = ::DDLoadBitmap(g_pDD, "image/End.bmp", HRES, VRES, TRUE);
	//// VRAM Cache 생성, 컬러키2
	::__CreateFastVBuffer(HRES, VRES, RGB(255, 255, 0));
	::__CreateFastSBuffer(HRES, VRES, RGB(255, 255, 0));
	::__CreateFastSBuffer2(HRES, VRES, RGB(255, 255, 0));

	g_Pos.x = 20;
	g_Pos.y = 590;
	g_BPos.x = 700;
	g_BPos.y = 530;
	Timer[7] = 60;
	Timer[17] = 150;
	Timer[20] = 300;
	Timer[28] = 500;
	//// LUT
	DXW_LUTInit();

	srand(time(NULL));
	///// 기타
	gFpsMng.SetFPS(_FPS);
	srand((unsigned)time(NULL));
	__CreateFont("bahnschrift", FNTSZ, FNTWH);	

#ifdef _DISPLAYINFO
	gAppSet.bIsDisplayInfo = TRUE;
#endif

	TRACE0("DXW_Init()\n");
    return TRUE;
}

void DXW_MainLoop()
{
	HRESULT hRet;

	//////////////////////////////////////////////////////////		
	RECT rcBox = { 100, 100, 300, 300 };

	//// 연산처리
	WORD wPixel = __GetPixelRGB16(0, 0, 0);
	int nX, bnX; //플레이어와 보스의 X좌표
	int nY, bnY; //플레이어와 보스의 Y좌표
	int SnX, SnY; //스킬을 표시할 X,Y좌표.
	int i = 0;
	if (g_nPhaze >= 1 && Knockback == FALSE && PlayerHP > 1 && g_nPhaze < 4)
	{
		if (::GetKeyState(VK_LEFT) & 0x80 && g_bJump == 0 && g_bAtt == FALSE && g_bRush == FALSE) //왼쪽 이동 로직
		{
			Hspeed -= 2;
			g_bAccel = TRUE;
			g_bLeft = TRUE;
			g_bRight = FALSE;
			g_bLookL = TRUE;
			g_bLookR = FALSE;
		}
		else if (g_Pos.x > 0 && g_bJump == 1) //점프 중 가속
		{
			if (g_bJLeft == 1)
			{
				if (g_bSetSpeed == 0)
				{
					Hspeed -= 1;
				}
				/*if (g_bJaccel == 1)
				{
				Hspeed -= 1;
				}*/
			}
			else if (g_bJRight == 0 && ::GetKeyState(VK_LEFT) & 0x80) //점프 중 가속
			{
				if (Hspeed > -2)
				{
					Hspeed -= 1;
				}
			}
		}
		if (g_bJLeft == 1 && ::GetKeyState(VK_RIGHT) & 0x80) //점프 중 반대 키를 눌렀을때 감속.
		{
			if (Hspeed > -3)
			{
				Hspeed += 1;
				g_bSetSpeed = TRUE; //감속 한계치 설정
			}
			else
			{
				Hspeed = -3;
			}
		}
		//////////////////
		if (::GetKeyState(VK_RIGHT) & 0x80 && g_bJump == 0 && g_bAtt == FALSE && g_bRush == FALSE) //오른쪽 이동 로직
		{
			Hspeed += 1;
			g_bAccel = TRUE;
			g_bLeft = FALSE;
			g_bRight = TRUE;
			g_bLookR = TRUE;
			g_bLookL = FALSE;
		}
		else if (g_Pos.x < HRES - 50 && g_bJump == 1)
		{
			if (g_bJRight == 1)
			{
				if (g_bSetSpeed == 0)
				{
					Hspeed += 1;
				}
				/*if (g_bJaccel == 1)
				{
				Hspeed += 1;
				}*/
			}
			else if (g_bJLeft == 0 && ::GetKeyState(VK_RIGHT) & 0x80)
			{
				if (Hspeed < 2)
				{
					Hspeed += 1;
				}
			}
		}
		if (g_bJRight == 1 && ::GetKeyState(VK_LEFT) & 0x80)
		{
			if (Hspeed > 3)
			{
				Hspeed -= 1;
				g_bSetSpeed = TRUE;
			}
			else
			{
				Hspeed = 3;
			}
		}
		if (g_bAtt == TRUE && g_bJump == FALSE)
		{
			if (Hspeed > 0)
			{
				Hspeed -= 1;
			}
			else if (Hspeed < 0)
			{
				Hspeed += 1;
			}
		}
		if (g_bRush == TRUE)
		{
			if (Hspeed > 0)
			{
				Hspeed = 0;
			}
			else if (Hspeed < 0)
			{
				Hspeed = 0;
			}
		}
		///////////////////////
		///////////////////////
		///////////////////////
		///////////////////////
		//아무 키도 입력하지 않았을 경우 감속
		else if (::GetKeyState(VK_LEFT) & 0x80) {}
		else if (::GetKeyState(VK_RIGHT) & 0x80) {}
		else {
			g_bAccel = FALSE; g_bLeft = FALSE; g_bRight = FALSE;
			if (Hspeed > 0)
			{
				Hspeed -= 1;
			}
			else if (Hspeed < 0)
			{
				Hspeed += 1;
			}
		}
		//점프 로직
		if (::GetKeyState(0x5A) & 0x80 && g_bJump == FALSE && g_bAtt == FALSE)
		{
			g_bJump = TRUE;
			if (g_bAccel == 1)
			{
				g_bJaccel = TRUE;
			}
			if (g_bRight == 1)
			{
				g_bJRight = TRUE;
				g_bJLeft = FALSE;
			}
			if (g_bLeft == 1)
			{
				g_bJLeft = TRUE;
				g_bJRight = FALSE;
			}
		}
		//가속도 최대치 설정
		if (Hspeed > 5)
		{
			Hspeed = 5;
		}
		else if (Hspeed < -5)
		{
			Hspeed = -5;
		}
		if (::GetKeyState(0x43) & 0x80 && g_bJump == FALSE && g_bRush == FALSE && Timer[9] == 0 && g_bLookR == TRUE)
		{
			if (g_bAtt == TRUE)
			{
				Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE;
				if (g_pDS[3])
					m_HHSound.Stop(g_pDS[3]);
			}
			g_bRush = TRUE;
			g_bRushR = TRUE;
			Cframe = 7;
			if (g_pDS[4])
				m_HHSound.Stop(g_pDS[4]);
			m_HHSound.Play(g_pDS[4], false);
		}
		else if (::GetKeyState(0x43) & 0x80 && g_bJump == FALSE && g_bRush == FALSE && Timer[9] == 0 && g_bLookL == TRUE)
		{
			if (g_bAtt == TRUE)
			{
				Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE;
				if (g_pDS[3])
					m_HHSound.Stop(g_pDS[3]);
			}
			g_bRush = TRUE;
			g_bRushL = TRUE;
			Cframe = 0;
			if (g_pDS[4])
				m_HHSound.Stop(g_pDS[4]);
			m_HHSound.Play(g_pDS[4], false);
		}
		if (g_bRush == TRUE)
		{
			if (Cframe < 8 && g_bRushR)
			{
				if (Cframe > 5)
				{
					g_Pos.x += 32;
				}
				Timer[8]++;
				if (Timer[8] == 4 && Cframe > 0)
				{
					Timer[8] = 0;
					Cframe--;
				}
				if (Cframe == 0)
				{
					if (Timer[8] > 8)
					{
						g_bRush = FALSE;
						g_bRushR = FALSE;
						Timer[9] = 50;
						Timer[8] = 0;
						Cframe = 0;
					}
					else
					{
						Timer[8]++;
					}
				}
			}
			if (Cframe < 8 && g_bRushL)
			{
				if (Cframe < 2)
				{
					g_Pos.x -= 32;
				}
				Timer[8]++;
				if (Timer[8] == 4 && Cframe < 7)
				{
					Timer[8] = 0;
					Cframe++;
				}
				if (Cframe == 7)
				{
					if (Timer[8] > 8)
					{
						g_bRush = FALSE;
						g_bRushL = FALSE;
						Timer[9] = 50;
						Timer[8] = 0;
						Cframe = 0;
					}
					else
					{
						Timer[8]++;
					}
				}
			}

		}
		if (Timer[9] > 0)
		{
			Timer[9]--;
		}
		g_Pos.x += Hspeed;
		//화면 밖으로 이동하는것을 제한함.
		//중력 관련 물리연산
	}
	if (g_bJump)
	{
		if (Gravity > -20)
		{
			Gravity -= 1;
			g_Pos.y -= Gravity;
			if (g_Pos.y > 590 && g_nPhaze == 1) //바닥
			{
				g_Pos.y = 590;
				Gravity = 13;
				g_bSetSpeed = FALSE;
				g_bJump = FALSE;
				g_bJaccel = FALSE;
				g_bJLeft = FALSE;
				g_bJRight = FALSE;
			}
			if (g_Pos.y > 500 && g_nPhaze == 2) //바닥
			{
				g_Pos.y = 500;
				Gravity = 13;
				g_bSetSpeed = FALSE;
				g_bJump = FALSE;
				g_bJaccel = FALSE;
				g_bJLeft = FALSE;
				g_bJRight = FALSE;
			}
			if (g_Pos.y > 510 && g_nPhaze == 3) //바닥
			{
				g_Pos.y = 510;
				Gravity = 13;
				g_bSetSpeed = FALSE;
				g_bJump = FALSE;
				g_bJaccel = FALSE;
				g_bJLeft = FALSE;
				g_bJRight = FALSE;
			}

		}
	}
	if (Knockback == TRUE)
	{
		g_Pos.x += Knockspd;
	}
	if (Knockback == TRUE && Knockspd > 0)
	{
		Knockspd -= 2;
		if (Knockspd <= 0)
		{
			Knockspd = 0;
		}
	}
	else if (Knockback == TRUE && Knockspd < 0)
	{
		Knockspd += 2;
		if (Knockspd >= 0)
		{
			Knockspd = 0;
		}
	}
	if (Knockback == TRUE && Knockspd == 0 && Delay == FALSE)
	{
		Timer[18] = 20;
		Delay = TRUE;
	}
	if (Timer[18] > 0 && Knockback == TRUE && Delay ==TRUE)
	{
		Timer[18] -= 1;
	}
	else if (Timer[18] == 0 && Knockback == TRUE && Delay ==TRUE) 
	{
		Knockback = FALSE;
		Delay = FALSE;
		Hspeed = 0;
		Timer[18] = 0;
	}
	if (g_Pos.x < 0)
	{
		g_Pos.x = 0;
	}
	if (g_Pos.x > HRES - 50)
	{
		g_Pos.x = HRES - 50;
	}
	if (g_nPhaze > 0 && g_nPhaze < 4)
	{
		if (Timer[28] > 0)
		{
			Timer[28]--;
		}
		if (::GetKeyState(VK_SPACE) & 0x80 && Timer[28] == 0 && Skilluse == FALSE)
		{
			Timer[28] = 500;
			MaxTime = 500;
			Timer[29] = 3;
			Skilluse = TRUE;
			if (g_pDS[5])
				m_HHSound.Stop(g_pDS[5]);
			m_HHSound.Play(g_pDS[5], false);
		}
	}
	if (Skilluse == TRUE)
	{
		if (Timer[29] == 0)
		{
			if (PSframe < 8)
			{
				Timer[29] = 4;
			}
			else if (PSframe > 8)
			{
				Timer[29] = 3;
			}
			PSframe++;
			if (PSframe > 0 && PSframe < 8)
			{
				if (p1die == FALSE && g_nPhaze == 1)
				{
					P1HP -= 15;
				}
				else if (p2die == FALSE && g_nPhaze == 2)
				{
					P2HP -= 20;
				}
				else if (p3die == FALSE && g_nPhaze == 3)
				{
					P3HP -= 40;
				}
			}
			else if (PSframe == 9 && BigHit == FALSE)
			{
				if (g_pDS[6])
					m_HHSound.Stop(g_pDS[6]);
				m_HHSound.Play(g_pDS[6], false);
				if (p1die == FALSE)
				{
					P1HP -= 150;
					if (Skillcheat > 0)
					{
						P1HP -= Skillcheat;
					}
				}
				else if (p2die == FALSE)
				{
					P2HP -= 200;
					if (Skillcheat > 0)
					{
						P2HP -= Skillcheat;
					}
				}
				else if (p3die == FALSE)
				{
					P3HP -= 400;
					if (Skillcheat > 0)
					{
						P3HP -= Skillcheat;
					}
				}
				BigHit = TRUE;
			}
		}
		else { Timer[29]--; }
		if (PSframe > 19)
		{
			Skilluse = FALSE;
			Timer[29] = 0;
			PSframe = 0;
			BigHit = FALSE;
		}
	}

	switch (g_nPhaze)
	{

	case 0:
		__PutImage(g_pDDSTITLE, 0, 0);
		if (::GetKeyState(VK_SPACE) & 0x80 && g_nPhaze == 0)
		{
			g_nPhaze = 1;
			Timer[12] = 300;
		}
		break;
	case 1:
		if (musicstart == TRUE)
		{
		}
			else
			{
				m_HHSound.Play(g_pDS[0], true);
				musicstart = TRUE;
			}
			//배경
			::__PutImage(g_pDDSBG, Bx, 0);
			if (Timer[11] > 0)
			{
				Timer[11] -= 1;
			}
			if (Timer[12] > 0)
			{
				Timer[12] -= 1;
			}
			if (g_bCast == FALSE && g_bCFire == FALSE && Timer[12] == 0 && BossAtt == FALSE)
			{
				g_bCast = TRUE;
				Timer[10] = 5;
			}
			//이동 및 점프 로직
			///////////////////////
			///////////////////////
			///////////////////////
			///////////////////////
			//				g_BPos.x += 1;

				if (g_BPos.x + 60 < g_Pos.x && g_bCast == FALSE) //보스 오른쪽 전환
				{
					g_bBLR = TRUE;
					g_bBLL = FALSE;
				}
				else if (g_BPos.x > g_Pos.x && g_bCast == FALSE) //보스 왼쪽 전환
				{
					g_bBLR = FALSE;
					g_bBLL = TRUE;
				}
				if (g_BPos.x + 300 < g_Pos.x && g_bCast == FALSE) //보스 오른쪽 이동
				{
					if (BossAtt == FALSE)
					{
						g_BPos.x += 1;
					}
					g_bBLR = TRUE;
					g_bBLL = FALSE;
				}
				else if (g_BPos.x - 200 > g_Pos.x && g_bCast == FALSE) //보스 왼쪽 이동
				{
					if (BossAtt == FALSE)
					{
						g_BPos.x -= 1;
					}
					g_bBLR = FALSE;
					g_bBLL = TRUE;
				}
				if (::GetKeyState(0x58) & 0x80 && g_bAtt == FALSE && g_bRush == FALSE) //공격 로직
			{
				if (g_bLookR == TRUE)
				{
					g_bAR = TRUE;
					g_bAtt = TRUE;
					Aframe = 12;
					if (g_pDS[3])
						m_HHSound.Stop(g_pDS[3]);
					m_HHSound.Play(g_pDS[3], false);
				}
				else if (g_bLookL == TRUE)
				{
					g_bAL = TRUE;
					g_bAtt = TRUE;
					Aframe = 0;
					if (g_pDS[3])
						m_HHSound.Stop(g_pDS[3]);
					m_HHSound.Play(g_pDS[3], false);
				}
			}
			//이동 및 점프 로직 끝
			//// 표면 클리어	
			//_FillSurface(g_pDDSBack, 0x00000000);
			nX = g_Pos.x;
			nY = g_Pos.y;
			bnX = g_BPos.x;
			bnY = g_BPos.y;
			SnX = nX;
			SnY = nY - 65;
			//플레이어 피격렉트 설정
			if (g_bLookL)
			{
				PHIT = { nX + 11,nY + 10,nX + 45,nY + 80 };
			}
			else if (g_bLookR)
			{
				PHIT = { nX + 25,nY + 10,nX + 58,nY + 80 };
			}
			//캐스팅 애니메이션
			if (g_bCast == TRUE && g_bBLL ==TRUE)
			{
				::__PutAnimeSprite(g_pDDSCast, bnX, bnY - 30, 20, CAframe, 192, 192, 0);
			}
			if (g_bCast == TRUE && g_bBLR == TRUE)
			{
				::__PutAnimeSprite(g_pDDSCast, bnX-20, bnY - 30, 20, CAframe, 192, 192, 0);
			}
			if (Timer[10] > 0 && g_bCast == TRUE)
			{
				Timer[10] -= 1;
			}
			else if (Timer[10] <= 0 && CAframe <= 19 && g_bCast == TRUE)
			{
				CAframe++;
				Timer[10] = 2;
			}
			if (CAframe == 20)
			{
				Timer[10] = 0;
				CAframe = 0;
				g_bCast = FALSE;
				Timer[12] = (rand() % 200) + 250;
				g_bCFire = TRUE;
			}
			//발사된 마법 애니메이션
			if (g_bCFire == TRUE && g_bBLL == TRUE && g_bCRight == FALSE && g_bCLeft == FALSE)
			{
				cx = bnX - 130;
				cy = bnY - 330;
				g_bCLeft = TRUE;
				Timer[11] = 3;
			}
			if (g_bCFire == TRUE && g_bBLR == TRUE && g_bCRight == FALSE && g_bCLeft == FALSE)
			{
				cx = bnX + 30;
				cy = bnY - 330;
				g_bCRight = TRUE;
				Timer[11] = 3;
			}
			if (g_bCLeft == TRUE)
			{
				if (Timer[11] > 0)
				{
					Timer[11] -= 1;
				}
				else if (Timer[11] == 0)
				{
					CAAframe += 1;
					Timer[11] = 3;
				}
				cx -= 7;
			}
			if (g_bCRight == TRUE)
			{
				if (Timer[11] > 0)
				{
					Timer[11] -= 1;
				}
				else if (Timer[11] == 0)
				{
					CAAframe += 1;
					Timer[11] = 3;
				}
				cx += 7;
			}
			if (CAAframe == 10)
			{
				CAAframe = 0;
			}
			if (cx < -100 || cx > HRES -200 && g_bCFire == TRUE)
			{
				g_bCFire = FALSE;
				g_bCLeft = FALSE;
				g_bCRight = FALSE;
				CastHit = FALSE;
				Timer[11] = 0;
				cx = 0;
				CAAframe = 0;
				Cast = {0,0,1,1};
			}
			else if (cx > -100 && cx < HRES - 200 && g_bCFire == TRUE)
			{
				Cast = { cx + 100,cy + 50,cx + 240,cy + 500 };
			}
			if (g_bCFire == TRUE)
			{
				::__PutAnimeSprite(g_pDDSCastAtt, cx,cy, 10, CAAframe, 344, 522, 0);
			}

			if (IntersectRect(&rcDest, &Cast, &PHIT) && CastHit == FALSE)
			{
				PlayerHP -= 2000;
				Cast = { 0,0,1,1 };
				CastHit = TRUE;
				if (PHplay == FALSE)
				{
					PHplay = TRUE;
					Timer[25] = 3;
					PHframe = 0;
				}
				else if (PHplay == TRUE)
				{
					PHplay = TRUE;
					Timer[25] = 3;
					PHframe = 0;
				}
			}
			//// 보스 스탠딩
			if (Timer[17] == 0 && BossAtt == FALSE)
			{ 
				Timer[16] = 4;
				BossAtt = TRUE;
				if (g_bBLL == TRUE)
				{
					BKframe = 0;
					BAL = TRUE;
					BAR = FALSE;
				}
				else { BKframe = 11; BAR = TRUE; BAL = FALSE; }
			}
			else if (Timer[17] > 0)
			{
				Timer[17] -= 1;
			}
			if (BossAtt == TRUE)
			{
				if (BAL == TRUE)
				{
					::__PutAnimeSprite(g_pDDSBossAtt, bnX-273, bnY-111, 12, BKframe, 593, 321, 0);
				}
				else if (BAR == TRUE)
				{
					::__PutAnimeSprite(g_pDDSBossAttR, bnX-147, bnY-111, 12, BKframe, 593, 321, 0);
				}
				if (Timer[16] > 0)
				{
					Timer[16] -= 1;
				}
				else if (Timer[16] == 0)
				{
					if (BAL == TRUE)
					{
						BKframe++;
					}
					else if (BAR == TRUE)
					{
						BKframe--;
					}
					if (BKframe < 5 && BAL == TRUE)
					{
						Timer[16] = 8;
					}
					else if (BKframe > 5 && BAL == TRUE)
					{
						Timer[16] = 5;
					}
					if (BKframe >8 && BAR == TRUE)
					{
						Timer[16] = 8;
					}
					else if (BKframe < 8 && BAR == TRUE)
					{
						Timer[16] = 5;
					}
				}
				if (BKframe == 12 && BAL == TRUE)
				{
					Timer[17] = (rand() % 150) + 200;
					Timer[16] = 0;
					BossAtt = FALSE;
					BAL = FALSE;
					BAR = FALSE;
					KnockHIT = FALSE;
					BKframe = 0;
					Knock = { 0,0,1,1 };
				}
				if (BKframe == 0 && BAR == TRUE)
				{
					Timer[17] = (rand ()% 150)+ 200;
					Timer[16] = 0;
					BossAtt = FALSE;
					BAL = FALSE;
					BAR = FALSE;
					BKframe = 0;
					KnockHIT = FALSE;
					Knock = { 0,0,1,1 };
				}
			}
			if (BKframe > 4 && BAL == TRUE && KnockHIT == FALSE)
			{
				Knock = { bnX - 183 ,bnY - 20,bnX + 40,bnY + 140 };
			}
			if (BKframe <7 && BAR == TRUE && KnockHIT == FALSE)
			{
				Knock = { bnX + 130 ,bnY - 20,bnX + 353,bnY + 140 };
			}
			if (BKframe > 6 && BAL == TRUE)
			{
				Knock = {0,0,1,1 };
			}
			if (BKframe <6 && BAR == TRUE)
			{
				Knock = {0,0,1,1};
			}
			if (IntersectRect(&rcDest, &Knock, &PHIT))
			{
				if (BAR == TRUE && Knockback == FALSE)
				{
					Knockspd = 30;
					Knockback = TRUE;
					PlayerHP -= 1000;
					KnockHIT = TRUE;
					Knock = { 0,0,1,1 };
					if (PHplay == FALSE)
					{
						PHplay = TRUE;
						Timer[25] = 3;
						PHframe = 0;
					}
					else if (PHplay == TRUE)
					{
						PHplay = TRUE;
						Timer[25] = 3;
						PHframe = 0;
					}
					if (g_bRush == TRUE)
					{
						g_bRush = FALSE;
						g_bRushL = FALSE;
						g_bRushR = FALSE;
						Timer[9] = 20;
						Timer[8] = 0;
						Cframe = 0;
					}

					if (g_bJump == TRUE)
					{
						Knockspd = 35;
					}
				}
				else if (BAL == TRUE && Knockback == FALSE)
				{
					Knockspd = -30;
					Knockback = TRUE;
					KnockHIT = TRUE;
					PlayerHP -= 1000;
					Knock = { 0,0,1,1 };
					if (PHplay == FALSE)
					{
						PHplay = TRUE;
						Timer[25] = 3;
						PHframe = 0;
					}
					if (g_bRush == TRUE)
					{
						g_bRush = FALSE;
						g_bRushL = FALSE;
						g_bRushR = FALSE;
						Timer[9] = 20;
						Timer[8] = 0;
						Cframe = 0;
					}
					if (g_bJump == TRUE)
					{
						Knockspd = -35;
					}
				}
			}
			if (p1die == FALSE && BossAtt == FALSE)
			{
				BOSS = { bnX + 10 ,bnY,bnX + 170,bnY + 148 };
				if (Timer[1] >= 8)
				{
					Timer[1] = 0;
					Bframe++;
					if (Bframe >= 7)
					{
						Bframe = 0;
					}
				}
				else { Timer[1]++; }
				if (g_bBLL == TRUE)
				{
					::__PutAnimeSprite(g_pDDSTwo, bnX, bnY, 8, Bframe, 174, 148, 0);
				}
				else if (g_bBLR == TRUE)
				{
					::__PutAnimeSprite(g_pDDSTwoR, bnX, bnY, 8, Bframe, 174, 148, 0);
				}
			}
			if (Timer[0] >= 26)
			{
				Timer[0] = 0;
			}
			//캐릭터 돌진 애니메이션
			if (g_bRush == FALSE)
			{
				//스탠딩
				if (g_bLookL == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
				}
				else if (g_bLookR == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
				}
				//점프
				if (g_bLookL == 1 && g_bJump == 1 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 3, 86, 81, 0);
				}
				else if (g_bLookR == 1 && g_bJump == 1 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 5, 86, 81, 0);
				}
				//무빙
				if (g_bLeft == 1 && g_bJump == 0 && Timer[0] < 5 && g_bJump == 0 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 1, 86, 81, 0);
				}
				else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 5 && Timer[0] < 12 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
				}
				else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 12 && Timer[0] < 19 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 2, 86, 81, 0);
				}
				else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 19 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
				}
				if (g_bRight == 1 && g_bJump == 0 && Timer[0] < 5 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 7, 86, 81, 0);
				}
				else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 5 && Timer[0] < 12 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
				}
				else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 12 && Timer[0] < 19 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 6, 86, 81, 0);
				}
				else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 19 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
				}
				if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] < 7)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 1, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 7 && Timer[4] < 14)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 2, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 14 && Timer[4] < 21)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 3, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 21 && Timer[4] < 27)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 4, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 27 && Timer[4] < 39)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 0, 86, 81, 0);
				}
				if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] < 7)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 4, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 7 && Timer[4] < 14)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 5, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 14 && Timer[4] < 21)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 6, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 21 && Timer[4] < 27)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 7, 86, 81, 0);
				}
				else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 27 && Timer[4] < 39)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 8, 86, 81, 0);
				}
				if (g_bAtt == TRUE && Timer[4] < 38)
				{
					Timer[4]++;
				}
				else
				{
					Timer[4] = 0;
				}
			}
			if (g_bRushL == TRUE)
			{
				::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 7, 86, 81, 0);
				::__PutAnimeSprite(g_pDDSCHARGE, nX - 170, nY - 100, 8, Cframe, 511, 263, 0);
			}
			else if (g_bRushR == TRUE)
			{
				::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 1, 86, 81, 0);
				::__PutAnimeSprite(g_pDDSCHARGER, nX - 250, nY - 100, 8, Cframe, 511, 263, 0);
			}
			if (g_bAtt == TRUE)
			{
				if (g_bAL == TRUE)
				{
					if (g_bHit == FALSE)
					{
						HIT = { SnX - 130,SnY + 10,SnX + 90,SnY + 180 };
					}
					else
					{
						HIT = { 0, 0, 1, 1 };
					}
					if (Timer[2] < 2)
					{
						Timer[2]++;
					}
					else
					{
						Timer[2] = 0;
						if (Aframe < 12)
						{
							Aframe++;
						}
						else { Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE;}
					}
					::__PutAnimeSprite(g_pDDSSkill, SnX - 140, SnY, 13, Aframe, 301, 218, 0);
				}
				else if (g_bAR == TRUE)
				{
					if (g_bHit == FALSE)
					{
						HIT = { SnX - 10,SnY + 10,SnX + 220,SnY + 180 };
					}
					else
					{
						HIT = { 0, 0, 1, 1 };
					}
					if (Timer[2] < 2)
					{
						Timer[2]++;
					}
					else
					{
						Timer[2] = 0;
						if (Aframe > 0)
						{
							Aframe--;
						}
						else { Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE;}
					}
					::__PutAnimeSprite(g_pDDSSkillR, SnX - 70, SnY, 13, Aframe, 301, 218, 0);
				}
			}
			if (bossimmu == FALSE && g_bHit == FALSE)
			{
				if (IntersectRect(&rcDest, &BOSS, &HIT))
				{
					P1HP -= 25;
					bossimmu = TRUE;
					Timer[5] = 20;
					g_bHit = TRUE;
					if (BHMPlay == FALSE)
					{
						BHMPlay = TRUE;
						BHplay[0] = TRUE;
						Timer[24] = 2;
					}
					else if (BHMPlay == TRUE)
					{
						BHplay[0] = TRUE;
						BHplay[1] = FALSE;
						BHplay[2] = FALSE;
						BHframe[0] = 0;
						BHframe[1] = 0;
						BHframe[2] = 0;
						Timer[24] = 2;
						Timer[26] = 2;
						Timer[27] = 2;
					}
				}
			}
			if (g_bRight == 1 || g_bLeft == 1 && g_bJump == 0)
			{
				Timer[0] += 1;
			}
			else
			{
				Timer[0] = 0;
			}
			if (p1die == 0)
			{
				if (Timer[7] == 0)
				{
					if (g_bBAT == TRUE)
					{
						if (BAframe < 10 && Timer[6] < 3)
						{
							BAHIT[0] = { 35,200,115,700 };
							BAHIT[1] = { 255,200,335,700 };
							BAHIT[2] = { 475,200,555,700 };
							BAHIT[3] = { 695,200,775,700 };
							BAHIT[4] = { 915,200,995,700 };
							Timer[6]++;
						}
						else if (BAframe < 10 && Timer[6] >= 3)
						{
							BAframe++;
							Timer[6] = 0;
						}
						if (BAframe >= 13)
						{
							int i = 0;
							for (i = 0; i < 5; i++)
							{
								if (RandNum[i] == FALSE)
								{
									if (IntersectRect(&rcDest, &BAHIT[i], &PHIT))
									{
										PlayerHP -= 500;
										BAHIT[i] = { 0,0,1,1 };
										if (PHplay == FALSE)
										{
											PHplay = TRUE;
											Timer[25] = 3;
											PHframe = 0;
										}
										else if (PHplay == TRUE)
										{
											PHplay = TRUE;
											Timer[25] = 3;
											PHframe = 0;
										}
									}
								}
							}
						}
						if (BAframe >= 10 && Timer[6] < 2 && BAframe < 19)
						{
							Timer[6]++;
						}
						else if (BAframe >= 10 && Timer[6] >= 2 && BAframe < 19)
						{
							BAframe++;
							Timer[6] = 0;
						}
						switch (Randpat)
						{
						case 0:
							::__PutAnimeSprite(g_BossAtt, -170, 50, 20, BAframe, 600, 656, 0);
							::__PutAnimeSprite(g_BossAtt, 270, 50, 20, BAframe, 600, 656, 0);
							::__PutAnimeSprite(g_BossAtt, 710, 50, 20, BAframe, 600, 656, 0);
							break;
						case 1:
							::__PutAnimeSprite(g_BossAtt, 50, 50, 20, BAframe, 600, 656, 0);
							::__PutAnimeSprite(g_BossAtt, 490, 50, 20, BAframe, 600, 656, 0);
							break;
						case 2:
							if (RandNum[0] == TRUE)
							{
								::__PutAnimeSprite(g_BossAtt2, -170, 50, 20, BAframe, 600, 656, 0);
							}
							else
							{
								::__PutAnimeSprite(g_BossAtt, -170, 50, 20, BAframe, 600, 656, 0);
							}
							if (RandNum[1] == TRUE)
							{
								::__PutAnimeSprite(g_BossAtt2, 50, 50, 20, BAframe, 600, 656, 0);
							}
							else
							{
								::__PutAnimeSprite(g_BossAtt, 50, 50, 20, BAframe, 600, 656, 0);
							}
							if (RandNum[2] == TRUE)
							{
								::__PutAnimeSprite(g_BossAtt2, 270, 50, 20, BAframe, 600, 656, 0);
							}
							else
							{
								::__PutAnimeSprite(g_BossAtt, 270, 50, 20, BAframe, 600, 656, 0);
							}
							if (RandNum[3] == TRUE)
							{
								::__PutAnimeSprite(g_BossAtt2, 490, 50, 20, BAframe, 600, 656, 0);
							}
							else
							{
								::__PutAnimeSprite(g_BossAtt, 490, 50, 20, BAframe, 600, 656, 0);
							}
							if (RandNum[4] == TRUE)
							{
								::__PutAnimeSprite(g_BossAtt2, 710, 50, 20, BAframe, 600, 656, 0);
							}
							else
							{
								::__PutAnimeSprite(g_BossAtt, 710, 50, 20, BAframe, 600, 656, 0);
							}
							break;
						}
						if (BAframe >= 19)
						{
							if (Timer[6] > 2)
							{
								BAframe = 0;
								Randpat2 = Randpat;
								g_bBAT = FALSE;
								Timer[7] = 60;
								if (P1HP < 250)
								{
									Timer[7] -= 50;
								}
								Maximum = 0;
								RandNum[0, 1, 2, 3, 4] = { 0 };
							}
							else
							{
								Timer[6]++;
							}
						}
					}
					else
					{
						Randpat = rand() % 3;
						if (Randpat == Randpat2)
						{
							switch (Randpat)
							{
							case 0:
								Randpat = 1;
								break;
							case 1:
								Randpat = 2;
								break;
							case 2:
								Randpat = 0;
								break;
							}
						}
						if (Randpat == 2)
						{
							RandNum[0] = rand() % 2;
							if (RandNum[0] == 1 && Maximum < 2)
							{
								Maximum++;
							}
							else if (RandNum[0] == 1 && Maximum >= 2)
							{
								RandNum[0] = 0;
							}
							RandNum[1] = rand() % 2;
							if (RandNum[1] == 1 && Maximum < 2)
							{
								Maximum++;
							}
							else if (RandNum[1] == 1 && Maximum >= 2)
							{
								RandNum[1] = 0;
							}
							RandNum[2] = rand() % 2;
							if (RandNum[2] == 1 && Maximum < 2)
							{
								Maximum++;
							}
							else if (RandNum[2] == 1 && Maximum >= 2)
							{
								RandNum[2] = 0;
							}
							RandNum[3] = rand() % 2;
							if (RandNum[3] == 1 && Maximum < 2)
							{
								Maximum++;
							}
							else if (RandNum[3] == 1 && Maximum >= 2)
							{
								RandNum[3] = 0;
							}
							RandNum[4] = rand() % 2;
							if (RandNum[4] == 1 && Maximum < 2)
							{
								Maximum++;
							}
							else if (RandNum[4] == 1 && Maximum >= 2)
							{
								RandNum[4] = 0;
							}
							if (Maximum == 0)
							{
								int n = 0;
								n = rand() % 4;
								RandNum[n] = 1;
							}
						}
						switch (Randpat)
						{
						case 0:
							RandNum[0] = 0;
							RandNum[1] = 1;
							RandNum[2] = 0;
							RandNum[3] = 1;
							RandNum[4] = 0;
							break;
						case 1:
							RandNum[0] = 1;
							RandNum[1] = 0;
							RandNum[2] = 1;
							RandNum[3] = 0;
							RandNum[4] = 1;
							break;
						}
						g_bBAT = TRUE;
					}
				}
			}
			if (PHplay == TRUE)
			{
				if (Timer[25] == 0)
				{
					PHframe++;
					Timer[25] = 3;
					if (PHframe > 5)
					{
						PHplay = FALSE;
						PHframe = 0;
						Timer[25] = 0;
					}
				}
				else { Timer[25] --; }
				if (PHplay == TRUE)
				{
					if (g_bLookL)
					{
						::__PutAnimeSprite(g_pDDSDPlayerHit, nX - 50, nY - 70, 6, PHframe, 180, 226, 0);
					}
					if (g_bLookR)
					{
						::__PutAnimeSprite(g_pDDSDPlayerHit, nX - 40, nY - 70, 6, PHframe, 180, 226, 0);
					}
				}
			}
			if (BHMPlay == TRUE)
			{
				if (Timer[24] == 0 && BHplay[0] ==TRUE)
				{
					BHframe[0]++;
					Timer[24] = 2;
					if (BHframe[0] == 6)
					{
						BHframe[0] = 0;
						Timer[24] = 0;
						BHplay[0] = FALSE;
					}
					if (BHframe[0] == 4 && BHplay[1] == FALSE)
					{
						Timer[26] = 2;
						BHplay[1] = TRUE;
					}
				}
				else if (Timer[24] > 0 && BHplay[0] == TRUE) { Timer[24]--; }
				if (Timer[26] == 0 && BHplay[1] == TRUE)
				{
					BHframe[1]++;
					Timer[26] = 2;
					if (BHframe[1] == 6)
					{
						BHframe[1] = 0;
						Timer[26] = 0;
						BHplay[1] = FALSE;
					}
					if (BHframe[1] == 4 && BHplay[2] == FALSE)
					{
						Timer[27] = 2;
						BHplay[2] = TRUE;
					}
				}
				else if (Timer[26] > 0 && BHplay[1] == TRUE){ Timer[26]--; }
				if (Timer[27] == 0 && BHplay[2] == TRUE)
				{
					BHframe[2]++;
					Timer[27] = 2;
					if (BHframe[2] == 6)
					{
						BHframe[2] = 0;
						Timer[27] = 0;
						BHplay[2] = FALSE;
					}
				}
				else if (Timer[27] > 0 && BHplay[2] == TRUE) { Timer[27]--; }
				if (BHplay[0] == FALSE && BHplay[1] == FALSE && BHplay[2] == FALSE)
				{
					BHMPlay = FALSE;
				}
				if (BHplay[0] == TRUE)
				{
					if (g_bLookL)
					{
						::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[0], 180, 226, 0);
					}
					if (g_bLookR)
					{
						::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[0], 180, 226, 0);
					}
				}
				if (BHplay[1] == TRUE)
				{
					if (g_bLookL)
					{
						::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[1], 180, 226, 0);
					}
					if (g_bLookR)
					{
						::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[1], 180, 226, 0);
					}
				}
				if (BHplay[2] == TRUE)
				{
					if (g_bLookL)
					{
						::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[2], 180, 226, 0);
					}
					if (g_bLookR)
					{
						::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[2], 180, 226, 0);
					}
				}
			}
			if (P1HP < 1)
			{
				if (g_pDS[0])
					m_HHSound.Stop(g_pDS[0]);
				p1die = 1;
				g_nPhaze = 2;
				g_Pos.x = 0;
				g_Pos.y = 500;
				g_BPos.x = 700;
				g_BPos.y = 430;
				Gravity = 13;
				g_bSetSpeed = FALSE;
				g_bJump = FALSE;
				g_bJaccel = FALSE;
				g_bJLeft = FALSE;
				g_bJRight = FALSE;
				g_bCast = FALSE;
				g_bCFire = FALSE;
				g_bCLeft = FALSE;
				g_bCRight = FALSE;
				CastHit = FALSE;
				Timer[11] = 0;
				cx = 0;
				CAAframe = 0;
				Cast = { 0,0,1,1 };
				Timer[17] = (rand() % 150) + 200;
				Timer[16] = 0;
				BossAtt = FALSE;
				BAframe = 0;
				Randpat2 = Randpat;
				g_bBAT = FALSE;
				Timer[7] = 60;
				Maximum = 0;
				RandNum[0, 1, 2, 3, 4] = { 0 };
				BAL = FALSE;
				BAR = FALSE;
				BKframe = 0;
				KnockHIT = FALSE;
				Knock = { 0,0,1,1 };
				BHplay[0] = FALSE;
				BHplay[1] = FALSE;
				BHplay[2] = FALSE;
				BHMPlay = FALSE;
				BHframe[0] = 0;
				BHframe[1] = 0;
				BHframe[2] = 0;
				Timer[24] = 2;
				Timer[26] = 2;
				Timer[27] = 2;

			}
			//렉트 확인용
			//::__Lock(g_pDDSBack);
			//__DrawRect16(PHIT.left, PHIT.top, PHIT.right, PHIT.bottom, wPixel);
			//__DrawRect16(HIT.left, HIT.top, HIT.right, HIT.bottom, wPixel);
			//__DrawRect16(Cast.left, Cast.top, Cast.right, Cast.bottom, wPixel);
			//__DrawRect16(BOSS.left, BOSS.top, BOSS.right, BOSS.bottom, wPixel);
			//__DrawRect16(Knock.left, Knock.top, Knock.right, Knock.bottom, wPixel);
			//for (i = 0; i < 5; i++)
			//{
			//	__DrawRect16(BAHIT[i].left, BAHIT[i].top, BAHIT[i].right, BAHIT[i].bottom, wPixel);
			//}
			//::__Unlock(g_pDDSBack);
			if (Timer[7] > 0)
			{
				Timer[7]--;
			}
			if (Timer[5] > 0)
			{
				Timer[5] -= 1;
			}
			else if (Timer[5] <= 0 && bossimmu == TRUE)
			{
				bossimmu = FALSE;
				Timer[5] = 0;
			}
			::__PutSprite(g_pDDSHP, 130, 0);
			if (P1HP > 1)
			{
				::__Putbar(g_pDDSHPbar, 132, 2, P1MHP, P1HP, P1HPrate);
			}
			::__PutSprite(g_pDDSPHP, 400, 737);
			if (PlayerHP > 1)
			{
				::__Putbar(g_pDDSPHPbar, 425, 744, PlayerMHP, PlayerHP, PlayerHPrate);
			}
			//////////////////////////////////////////////////////////
			///// 기타정보 
			DXW_DisplayInfo();
			break;
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
				case 2 : if (P2musicstart == TRUE)
				{
				}
						else
						{
							m_HHSound.Play(g_pDS[1], true);
							P2musicstart = TRUE;
						}
						 //배경
							 ::__PutImage(g_pDDSBG2, Bx, 0);
						 if (Timer[11] > 0)
						 {
							 Timer[11] -= 1;
						 }
						 if (Timer[12] > 0)
						 {
							 Timer[12] -= 1;
						 }
						 else if (Timer[15] > 0)
						 {
							 Timer[15] -= 1;
						 }
						 //이동 및 점프 로직
						 ///////////////////////
						 ///////////////////////
						 ///////////////////////
						 ///////////////////////
						 //				g_BPos.x += 1;
						 if (g_BPos.x + 60 < g_Pos.x) //보스 오른쪽 전환
						 {
							 g_bBLR = TRUE;
							 g_bBLL = FALSE;
						 }
						 else if (g_BPos.x > g_Pos.x) //보스 왼쪽 전환
						 {
							 g_bBLR = FALSE;
							 g_bBLL = TRUE;
						 }
						 if (g_BPos.x + 300 < g_Pos.x) //보스 오른쪽 이동
						 {
							 if (BossAtt == FALSE)
							 {
								 g_BPos.x += 1;
							 }
							 g_bBLR = TRUE;
							 g_bBLL = FALSE;
						 }
						 else if (g_BPos.x - 200 > g_Pos.x) //보스 왼쪽 이동
						 {
							 if (BossAtt == FALSE)
							 {
								 g_BPos.x -= 1;
							 }
							 g_bBLR = FALSE;
							 g_bBLL = TRUE;
						 }
						 if (::GetKeyState(0x58) & 0x80 && g_bAtt == FALSE && g_bRush == FALSE) //공격 로직
						 {
							 if (g_bLookR == TRUE)
							 {
								 g_bAR = TRUE;
								 g_bAtt = TRUE;
								 Aframe = 12;
								 if (g_pDS[3])
									 m_HHSound.Stop(g_pDS[3]);
								 m_HHSound.Play(g_pDS[3], false);
							 }
							 else if (g_bLookL == TRUE)
							 {
								 g_bAL = TRUE;
								 g_bAtt = TRUE;
								 Aframe = 0;
								 if (g_pDS[3])
									 m_HHSound.Stop(g_pDS[3]);
								 m_HHSound.Play(g_pDS[3], false);
							 }
						 }
						 //이동 및 점프 로직 끝
						 //// 표면 클리어	
						 //_FillSurface(g_pDDSBack, 0x00000000);
						 nX = g_Pos.x;
						 nY = g_Pos.y;
						 bnX = g_BPos.x;
						 bnY = g_BPos.y;
						 SnX = nX;
						 SnY = nY - 65;
						 //캐스팅 애니메이션
						 if (Timer[14] >0)
						 { 
							 Timer[14] -= 1;
						 }
						 if (g_bCast == TRUE && g_bBLL == TRUE)
						 {
							 ::__PutAnimeSprite(g_pDDSCast2, bnX, bnY - 30, 20, CAframe, 192, 192, 0);
						 }
						 if (g_bCast == TRUE && g_bBLR == TRUE)
						 {
							 ::__PutAnimeSprite(g_pDDSCast2, bnX - 20, bnY - 30, 20, CAframe, 192, 192, 0);
						 }
						 if (Timer[10] > 0 && g_bCast == TRUE)
						 {
							 Timer[10] -= 1;
						 }
						 else if (Timer[10] <= 0 && CAframe <= 19 && g_bCast == TRUE)
						 {
							 CAframe++;
							 Timer[10] = 2;
						 }
						 if (CAframe == 20)
						 {
							 Timer[10] = 0;
							 CAframe = 0;
						 }
						 //플레이어 피격렉트 설정
						 if (g_bLookL)
						 {
							 PHIT = { nX + 11,nY + 10,nX + 45,nY + 80 };
						 }
						 else if (g_bLookR)
						 {
							 PHIT = { nX + 25,nY + 10,nX + 58,nY + 80 };
						 }
						 if (IntersectRect(&rcDest, &Cast, &PHIT))
						 {
							 PlayerHP -= 1000;
						 }
						 //// 보스 스탠딩

						 if (Timer[17] == 0 && BossAtt == FALSE)
						 {
							 Timer[16] = 4;
							 BossAtt = TRUE;
							 if (g_bBLL == TRUE)
							 {
								 BKframe = 0;
								 BAL = TRUE;
								 BAR = FALSE;
							 }
							 else { BKframe = 16; BAR = TRUE; BAL = FALSE; }
						 }
						 else if (Timer[17] > 0)
						 {
							 Timer[17] -= 1;
						 }
						 if (BossAtt == TRUE)
						 {
							 if (BAL == TRUE)
							 {
								 ::__PutAnimeSprite(g_pDDSDragAtt, bnX - 513, bnY - 99, 17, BKframe, 720, 315, 0);
							 }
							 else if (BAR == TRUE)
							 {
								 ::__PutAnimeSprite(g_pDDSDragAttR, bnX - 15, bnY - 99, 17, BKframe, 720, 315, 0);
							 }
							 if (Timer[16] > 0)
							 {
								 Timer[16] -= 1;
							 }
							 else if (Timer[16] == 0)
							 {
								 if (BAL == TRUE)
								 {
									 BKframe++;
								 }
								 else if (BAR == TRUE)
								 {
									 BKframe--;
								 }
								 if (BKframe < 8 && BAL == TRUE)
								 {
									 Timer[16] = 5;
								 }
								 else if (BKframe > 8 && BAL == TRUE)
								 {
									 Timer[16] = 4;
								 }
								 if (BKframe > 9 && BAR == TRUE)
								 {
									 Timer[16] = 5;
								 }
								 else if (BKframe < 9 && BAR == TRUE)
								 {
									 Timer[16] = 4;
								 }
							 }
							 if (BKframe == 16 && BAL == TRUE)
							 {
								 Timer[17] = (rand() % 150) + 200;
								 Timer[16] = 0;
								 BossAtt = FALSE;
								 BAL = FALSE;
								 BAR = FALSE;
								 KnockHIT = FALSE;
								 BKframe = 0;
								 Knock = { 0,0,1,1 };
							 }
							 if (BKframe == 0 && BAR == TRUE)
							 {
								 Timer[17] = (rand() % 150) + 200;
								 Timer[16] = 0;
								 BossAtt = FALSE;
								 BAL = FALSE;
								 BAR = FALSE;
								 BKframe = 0;
								 KnockHIT = FALSE;
								 Knock = { 0,0,1,1 };
							 }
						 }
						 if (BKframe > 7 && BAL == TRUE && KnockHIT == FALSE)
						 {
							 Knock = { bnX - 483 ,bnY - 20,bnX + 40,bnY + 140 };
						 }
						 if (BKframe < 8 && BAR == TRUE && KnockHIT == FALSE)
						 {
							 Knock = { bnX + 130 ,bnY - 20,bnX + 653,bnY + 140 };
						 }
						 if (BKframe > 12 && BAL == TRUE)
						 {
							 Knock = { 0,0,1,1 };
						 }
						 if (BKframe < 5 && BAR == TRUE)
						 {
							Knock = { 0,0,1,1 };
						 }
						 if (IntersectRect(&rcDest, &Knock, &PHIT))
						 {
							 if (BAR == TRUE && Knockback == FALSE)
							 {
								 Knockspd = 40;
								 Knockback = TRUE;
								 PlayerHP -= 1000;
								 KnockHIT = TRUE;
								 Knock = { 0,0,1,1 };
								 if (PHplay == FALSE)
								 {
									 PHplay = TRUE;
									 Timer[25] = 3;
									 PHframe = 0;
								 }
								 else if (PHplay == TRUE)
								 {
									 PHplay = TRUE;
									 Timer[25] = 3;
									 PHframe = 0;
								 }
								 if (g_bRush == TRUE)
								 {
									 g_bRush = FALSE;
									 g_bRushL = FALSE;
									 g_bRushR = FALSE;
									 Timer[9] = 20;
									 Timer[8] = 0;
									 Cframe = 0;
								 }

								 if (g_bJump == TRUE)
								 {
									 Knockspd = 45;
								 }
							 }
							 else if (BAL == TRUE && Knockback == FALSE)
							 {
								 Knockspd = -40;
								 Knockback = TRUE;
								 KnockHIT = TRUE;
								 PlayerHP -= 1000;
								 Knock = { 0,0,1,1 };
								 if (PHplay == FALSE)
								 {
									 PHplay = TRUE;
									 Timer[25] = 3;
									 PHframe = 0;
								 }
								 else if (PHplay == TRUE)
								 {
									 PHplay = TRUE;
									 Timer[25] = 3;
									 PHframe = 0;
								 }
								 if (g_bRush == TRUE)
								 {
									 g_bRush = FALSE;
									 g_bRushL = FALSE;
									 g_bRushR = FALSE;
									 Timer[9] = 20;
									 Timer[8] = 0;
									 Cframe = 0;
								 }
								 if (g_bJump == TRUE)
								 {
									 Knockspd = -45;
								 }
							 }
						 }
						 if (p2die == FALSE && BossAtt ==FALSE)
						 {
							 BOSS = { bnX + 10 ,bnY,bnX + 170,bnY + 148 };
							 if (Timer[1] >= 8)
							 {
								 Timer[1] = 0;
								 Bframe++;
								 if (Bframe >= 7)
								 {
									 Bframe = 0;
								 }
							 }
							 else { Timer[1]++; }
							 if (g_bBLL == TRUE)
							 {
								 ::__PutAnimeSprite(g_pDDSTwo, bnX, bnY, 8, Bframe, 174, 148, 0);
							 }
							 else if (g_bBLR == TRUE)
							 {
								 ::__PutAnimeSprite(g_pDDSTwoR, bnX, bnY, 8, Bframe, 174, 148, 0);
							 }
						 }
						 if (Timer[0] >= 26)
						 {
							 Timer[0] = 0;
						 }
						 //캐릭터 돌진 애니메이션
						 if (g_bRush == FALSE)
						 {
							 //스탠딩
							 if (g_bLookL == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
							 }
							 else if (g_bLookR == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
							 }
							 //점프
							 if (g_bLookL == 1 && g_bJump == 1 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 3, 86, 81, 0);
							 }
							 else if (g_bLookR == 1 && g_bJump == 1 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 5, 86, 81, 0);
							 }
							 //무빙
							 if (g_bLeft == 1 && g_bJump == 0 && Timer[0] < 5 && g_bJump == 0 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 1, 86, 81, 0);
							 }
							 else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 5 && Timer[0] < 12 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
							 }
							 else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 12 && Timer[0] < 19 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 2, 86, 81, 0);
							 }
							 else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 19 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
							 }
							 if (g_bRight == 1 && g_bJump == 0 && Timer[0] < 5 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 7, 86, 81, 0);
							 }
							 else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 5 && Timer[0] < 12 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
							 }
							 else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 12 && Timer[0] < 19 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 6, 86, 81, 0);
							 }
							 else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 19 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
							 }
							 if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] < 7)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 1, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 7 && Timer[4] < 14)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 2, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 14 && Timer[4] < 21)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 3, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 21 && Timer[4] < 27)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 4, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 27 && Timer[4] < 39)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 0, 86, 81, 0);
							 }
							 if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] < 7)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 4, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 7 && Timer[4] < 14)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 5, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 14 && Timer[4] < 21)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 6, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 21 && Timer[4] < 27)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 7, 86, 81, 0);
							 }
							 else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 27 && Timer[4] < 39)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 8, 86, 81, 0);
							 }
							 if (g_bAtt == TRUE && Timer[4] < 38)
							 {
								 Timer[4]++;
							 }
							 else
							 {
								 Timer[4] = 0;
							 }
						 }
						 if (g_bRushL == TRUE)
						 {
							 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 7, 86, 81, 0);
							 ::__PutAnimeSprite(g_pDDSCHARGE, nX - 170, nY - 100, 8, Cframe, 511, 263, 0);
						 }
						 else if (g_bRushR == TRUE)
						 {
							 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 1, 86, 81, 0);
							 ::__PutAnimeSprite(g_pDDSCHARGER, nX - 250, nY - 100, 8, Cframe, 511, 263, 0);
						 }
						 if (g_bAtt == TRUE)
						 {
							 if (g_bAL == TRUE)
							 {
								 if (g_bHit == FALSE)
								 {
									 HIT = { SnX - 130,SnY + 10,SnX + 90,SnY + 180 };
								 }
								 else
								 {
									 HIT = { 0, 0, 1, 1 };
								 }
								 if (Timer[2] < 2)
								 {
									 Timer[2]++;
								 }
								 else
								 {
									 Timer[2] = 0;
									 if (Aframe < 12)
									 {
										 Aframe++;
									 }
									 else { Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE; }
								 }
								 ::__PutAnimeSprite(g_pDDSSkill, SnX - 140, SnY, 13, Aframe, 301, 218, 0);
							 }
							 else if (g_bAR == TRUE)
							 {
								 if (g_bHit == FALSE)
								 {
									 HIT = { SnX - 10,SnY + 10,SnX + 220,SnY + 180 };
								 }
								 else
								 {
									 HIT = { 0, 0, 1, 1 };
								 }
								 if (Timer[2] < 2)
								 {
									 Timer[2]++;
								 }
								 else
								 {
									 Timer[2] = 0;
									 if (Aframe > 0)
									 {
										 Aframe--;
									 }
									 else { Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE; }
								 }
								 ::__PutAnimeSprite(g_pDDSSkillR, SnX - 70, SnY, 13, Aframe, 301, 218, 0);
							 }
						 }
						 if (bossimmu == FALSE && g_bHit == FALSE)
						 {
							 if (IntersectRect(&rcDest, &BOSS, &HIT))
							 {
								 P2HP -= 75;
								 bossimmu = TRUE;
								 Timer[5] = 20;
								 g_bHit = TRUE;
								 if (BHMPlay == FALSE)
								 {
									 BHMPlay = TRUE;
									 BHplay[0] = TRUE;
									 Timer[24] = 2;
								 }
								 else if (BHMPlay == TRUE)
								 {
									 BHplay[0] = TRUE;
									 BHplay[1] = FALSE;
									 BHplay[2] = FALSE;
									 BHframe[0] = 0;
									 BHframe[1] = 0;
									 BHframe[2] = 0;
									 Timer[24] = 2;
									 Timer[26] = 2;
									 Timer[27] = 2;
								 }
							 }
						 }
						 if (g_bRight == 1 || g_bLeft == 1 && g_bJump == 0)
						 {
							 Timer[0] += 1;
						 }
						 else
						 {
							 Timer[0] = 0;
						 }
						 if (p2die == 0)
						 {
							 if (Timer[7] == 0)
							 {
								 if (g_bBAT == TRUE)
								 {
									 if (BAframe < 10 && Timer[6] < 3)
									 {
										 BAHIT[0] = { 35,200,115,700 };
										 BAHIT[1] = { 255,200,335,700 };
										 BAHIT[2] = { 475,200,555,700 };
										 BAHIT[3] = { 695,200,775,700 };
										 BAHIT[4] = { 915,200,995,700 };
										 Timer[6]++;
									 }
									 else if (BAframe < 10 && Timer[6] >= 3)
									 {
										 BAframe++;
										 Timer[6] = 0;
									 }
									 if (BAframe >= 13)
									 {
										 int i = 0;
										 for (i = 0; i < 5; i++)
										 {
											 if (RandNum[i] == FALSE)
											 {
												 if (IntersectRect(&rcDest, &BAHIT[i], &PHIT))
												 {
													 if (Mirror == FALSE)
													 {
														 PlayerHP -= 500;
														 BAHIT[i] = { 0,0,1,1 };
														 if (PHplay == FALSE)
														 {
															 PHplay = TRUE;
															 Timer[25] = 3;
															 PHframe = 0;
														 }
														 else if (PHplay == TRUE)
														 {
															 PHplay = TRUE;
															 Timer[25] = 3;
															 PHframe = 0;
														 }
													 }
													 else if (Mirror == TRUE)
													 {
														 isSafe = TRUE;
													 }
												 }
											 }
										 }
									 if (Mirror == TRUE && isSafe == FALSE)
									 {
										 if (Pimmune == 0)
										 {
											 PlayerHP -= 1000;
											 Pimmune = 1;
											 if (PHplay == FALSE)
											 {
												 PHplay = TRUE;
												 Timer[25] = 3;
												 PHframe = 0;
											 }
											 else if (PHplay == TRUE)
											 {
												 PHplay = TRUE;
												 Timer[25] = 3;
												 PHframe = 0;
											 }
										 }
									 }
									 }
									 if (BAframe >= 10 && Timer[6] < 2 && BAframe < 19)
									 {
										 Timer[6]++;
									 }
									 else if (BAframe >= 10 && Timer[6] >= 2 && BAframe < 19)
									 {
										 BAframe++;
										 Timer[6] = 0;
									 }
									 switch (Randpat)
									 {
									 case 0:
										 ::__PutAnimeSprite(g_BossAtt, -170, 50, 20, BAframe, 600, 656, 0);
										 ::__PutAnimeSprite(g_BossAtt, 270, 50, 20, BAframe, 600, 656, 0);
										 ::__PutAnimeSprite(g_BossAtt, 710, 50, 20, BAframe, 600, 656, 0);
										 break;
									 case 1:
										 ::__PutAnimeSprite(g_BossAtt, 50, 50, 20, BAframe, 600, 656, 0);
										 ::__PutAnimeSprite(g_BossAtt, 490, 50, 20, BAframe, 600, 656, 0);
										 break;
									 case 2:
										 if (RandNum[0] == TRUE)
										 {
											 ::__PutAnimeSprite(g_BossAtt2, -170, 50, 20, BAframe, 600, 656, 0);
										 }
										 else
										 {
											 ::__PutAnimeSprite(g_BossAtt, -170, 50, 20, BAframe, 600, 656, 0);
										 }
										 if (RandNum[1] == TRUE)
										 {
											 ::__PutAnimeSprite(g_BossAtt2, 50, 50, 20, BAframe, 600, 656, 0);
										 }
										 else
										 {
											 ::__PutAnimeSprite(g_BossAtt, 50, 50, 20, BAframe, 600, 656, 0);
										 }
										 if (RandNum[2] == TRUE)
										 {
											 ::__PutAnimeSprite(g_BossAtt2, 270, 50, 20, BAframe, 600, 656, 0);
										 }
										 else
										 {
											 ::__PutAnimeSprite(g_BossAtt, 270, 50, 20, BAframe, 600, 656, 0);
										 }
										 if (RandNum[3] == TRUE)
										 {
											 ::__PutAnimeSprite(g_BossAtt2, 490, 50, 20, BAframe, 600, 656, 0);
										 }
										 else
										 {
											 ::__PutAnimeSprite(g_BossAtt, 490, 50, 20, BAframe, 600, 656, 0);
										 }
										 if (RandNum[4] == TRUE)
										 {
											 ::__PutAnimeSprite(g_BossAtt2, 710, 50, 20, BAframe, 600, 656, 0);
										 }
										 else
										 {
											 ::__PutAnimeSprite(g_BossAtt, 710, 50, 20, BAframe, 600, 656, 0);
										 }
										 break;
									 }
									 if (BAframe >= 19)
									 {
										 if (Timer[6] > 2)
										 {
											 BAframe = 0;
											 Randpat2 = Randpat;
											 g_bBAT = FALSE;
											 Timer[7] = 60;
											 if (P2HP < 2500)
											 {
												 Timer[7] -= 55;
											 }
											 Maximum = 0;
											 RandNum[0, 1, 2, 3, 4] = { 0 };
											 isSafe = FALSE;
											 Pimmune = 0;
											 if (g_bCast == FALSE && Timer[15] == 0)
											 {
												 g_bCast = TRUE;
												 Mirror = TRUE;
												 Timer[13] = 5;
												 Timer[14] = 500;
											 }
											 if (Timer[14] == 0 && g_bCast == TRUE)
											 {
												 g_bCast = FALSE;
												 CAframe = 0;
												 Timer[10] = 0;
												 Timer[15] = (rand() % 200) + 600;
												 Mirror = FALSE;
											 }
										 }
										 else
										 {
											 Timer[6]++;
										 }
									 }
								 }
								 else
								 {
									 Randpat = rand() % 3;
									 if (Randpat == Randpat2)
									 {
										 switch (Randpat)
										 {
										 case 0:
											 Randpat = 1;
											 break;
										 case 1:
											 Randpat = 2;
											 break;
										 case 2:
											 Randpat = 0;
											 break;
										 }
									 }
									 if (Randpat == 2)
									 {
										 RandNum[0] = rand() % 2;
										 if (RandNum[0] == 1 && Maximum < 2)
										 {
											 Maximum++;
										 }
										 else if (RandNum[0] == 1 && Maximum >= 2)
										 {
											 RandNum[0] = 0;
										 }
										 RandNum[1] = rand() % 2;
										 if (RandNum[1] == 1 && Maximum < 2)
										 {
											 Maximum++;
										 }
										 else if (RandNum[1] == 1 && Maximum >= 2)
										 {
											 RandNum[1] = 0;
										 }
										 RandNum[2] = rand() % 2;
										 if (RandNum[2] == 1 && Maximum < 2)
										 {
											 Maximum++;
										 }
										 else if (RandNum[2] == 1 && Maximum >= 2)
										 {
											 RandNum[2] = 0;
										 }
										 RandNum[3] = rand() % 2;
										 if (RandNum[3] == 1 && Maximum < 2)
										 {
											 Maximum++;
										 }
										 else if (RandNum[3] == 1 && Maximum >= 2)
										 {
											 RandNum[3] = 0;
										 }
										 RandNum[4] = rand() % 2;
										 if (RandNum[4] == 1 && Maximum < 2)
										 {
											 Maximum++;
										 }
										 else if (RandNum[4] == 1 && Maximum >= 2)
										 {
											 RandNum[4] = 0;
										 }
										 if (Maximum == 0)
										 {
											 int n = 0;
											 n = rand() % 4;
											 RandNum[n] = 1;
										 }
									 }
									 switch (Randpat)
									 {
									 case 0:
										 RandNum[0] = 0;
										 RandNum[1] = 1;
										 RandNum[2] = 0;
										 RandNum[3] = 1;
										 RandNum[4] = 0;
										 break;
									 case 1:
										 RandNum[0] = 1;
										 RandNum[1] = 0;
										 RandNum[2] = 1;
										 RandNum[3] = 0;
										 RandNum[4] = 1;
										 break;
									 }
									 g_bBAT = TRUE;
								 }
							 }
						 }
						 if (PHplay == TRUE)
						 {
							 if (Timer[25] == 0)
							 {
								 PHframe++;
								 Timer[25] = 3;
								 if (PHframe > 5)
								 {
									 PHplay = FALSE;
									 PHframe = 0;
									 Timer[25] = 0;
								 }
							 }
							 else { Timer[25] --; }
							 if (PHplay == TRUE)
							 {
								 if (g_bLookL)
								 {
									 ::__PutAnimeSprite(g_pDDSDPlayerHit, nX - 50, nY - 70, 6, PHframe, 180, 226, 0);
								 }
								 if (g_bLookR)
								 {
									 ::__PutAnimeSprite(g_pDDSDPlayerHit, nX - 40, nY - 70, 6, PHframe, 180, 226, 0);
								 }
							 }
						 }
						 if (BHMPlay == TRUE)
						 {
							 if (Timer[24] == 0 && BHplay[0] == TRUE)
							 {
								 BHframe[0]++;
								 Timer[24] = 2;
								 if (BHframe[0] == 6)
								 {
									 BHframe[0] = 0;
									 Timer[24] = 0;
									 BHplay[0] = FALSE;
								 }
								 if (BHframe[0] == 4 && BHplay[1] == FALSE)
								 {
									 Timer[26] = 2;
									 BHplay[1] = TRUE;
								 }
							 }
							 else if (Timer[24] > 0 && BHplay[0] == TRUE) { Timer[24]--; }
							 if (Timer[26] == 0 && BHplay[1] == TRUE)
							 {
								 BHframe[1]++;
								 Timer[26] = 2;
								 if (BHframe[1] == 6)
								 {
									 BHframe[1] = 0;
									 Timer[26] = 0;
									 BHplay[1] = FALSE;
								 }
								 if (BHframe[1] == 4 && BHplay[2] == FALSE)
								 {
									 Timer[27] = 2;
									 BHplay[2] = TRUE;
								 }
							 }
							 else if (Timer[26] > 0 && BHplay[1] == TRUE) { Timer[26]--; }
							 if (Timer[27] == 0 && BHplay[2] == TRUE)
							 {
								 BHframe[2]++;
								 Timer[27] = 2;
								 if (BHframe[2] == 6)
								 {
									 BHframe[2] = 0;
									 Timer[27] = 0;
									 BHplay[2] = FALSE;
								 }
							 }
							 else if (Timer[27] > 0 && BHplay[2] == TRUE) { Timer[27]--; }
							 if (BHplay[0] == FALSE && BHplay[1] == FALSE && BHplay[2] == FALSE)
							 {
								 BHMPlay = FALSE;
							 }
							 if (BHplay[0] == TRUE)
							 {
								 if (g_bLookL)
								 {
									 ::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[0], 180, 226, 0);
								 }
								 if (g_bLookR)
								 {
									 ::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[0], 180, 226, 0);
								 }
							 }
							 if (BHplay[1] == TRUE)
							 {
								 if (g_bLookL)
								 {
									 ::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[1], 180, 226, 0);
								 }
								 if (g_bLookR)
								 {
									 ::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[1], 180, 226, 0);
								 }
							 }
							 if (BHplay[2] == TRUE)
							 {
								 if (g_bLookL)
								 {
									 ::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[2], 180, 226, 0);
								 }
								 if (g_bLookR)
								 {
									 ::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[2], 180, 226, 0);
								 }
							 }
						 }
						 if (P2HP < 1)
						 {
							 if (g_pDS[1])
								 m_HHSound.Stop(g_pDS[1]);
							 p2die = 1;
							 g_nPhaze = 3;
							 g_Pos.x = 0;
							 g_Pos.y = 550;
							 g_BPos.x = 700;
							 g_BPos.y = 480;
							 Gravity = 13;
							 g_bSetSpeed = FALSE;
							 g_bJump = FALSE;
							 g_bJaccel = FALSE;
							 g_bJLeft = FALSE;
							 g_bJRight = FALSE;
							 g_bCast = FALSE;
							 g_bCFire = FALSE;
							 g_bCLeft = FALSE;
							 g_bCRight = FALSE;
							 CastHit = FALSE;
							 Timer[11] = 0;
							 cx = 0;
							 CAAframe = 0;
							 Cast = { 0,0,1,1 };
							 Timer[17] = (rand() % 150) + 200;
							 Timer[16] = 0;
							 BossAtt = FALSE;
							 BAL = FALSE;
							 BAR = FALSE;
							 BKframe = 0;
							 KnockHIT = FALSE;
							 Knock = { 0,0,1,1 };
							 BAframe = 0;
							 Randpat2 = Randpat;
							 g_bBAT = FALSE;
							 Timer[7] = 60;
							 Maximum = 0;
							 RandNum[0, 1, 2, 3, 4] = { 0 };
							 BHplay[0] = FALSE;
							 BHplay[1] = FALSE;
							 BHplay[2] = FALSE;
							 BHMPlay = FALSE;
							 BHframe[0] = 0;
							 BHframe[1] = 0;
							 BHframe[2] = 0;
							 Timer[24] = 2;
							 Timer[26] = 2;
							 Timer[27] = 2;

						 }
						 //렉트 확인용
						 //::__Lock(g_pDDSBack);
						// __DrawRect16(PHIT.left, PHIT.top, PHIT.right, PHIT.bottom, wPixel);
						// __DrawRect16(HIT.left, HIT.top, HIT.right, HIT.bottom, wPixel);
						// __DrawRect16(Cast.left, Cast.top, Cast.right, Cast.bottom, wPixel);
						// __DrawRect16(BOSS.left, BOSS.top, BOSS.right, BOSS.bottom, wPixel);
						 //__DrawRect16(Knock.left, Knock.top, Knock.right, Knock.bottom, wPixel);
						// for (i = 0; i < 5; i++)
						// {
						// __DrawRect16(BAHIT[i].left, BAHIT[i].top, BAHIT[i].right, BAHIT[i].bottom, wPixel);
						// }
						 //::__Unlock(g_pDDSBack);
						 
						 if (Timer[7] > 0)
						 {
							 Timer[7]--;
						 }
						 if (Timer[5] > 0)
						 {
							 Timer[5] -= 1;
						 }
						 else if (Timer[5] <= 0 && bossimmu == TRUE)
						 {
							 bossimmu = FALSE;
							 Timer[5] = 0;
						 }
						 ::__PutSprite(g_pDDSHP, 130, 0);
						 if (P2HP > 1)
						 {
							 ::__Putbar(g_pDDSHPbar, 132, 2, P2MHP, P2HP, P2HPrate);
						 }
						 ::__PutSprite(g_pDDSPHP, 400, 737);
						 if (PlayerHP > 1)
						 {
							 ::__Putbar(g_pDDSPHPbar, 425, 744, PlayerMHP, PlayerHP, PlayerHPrate);
						 }
						 //////////////////////////////////////////////////////////
						 ///// 기타정보 
						 DXW_DisplayInfo();
						 break;
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
						 //////////////////////////////////////////////////////////
				case 3 : 
					if (p3musicstart == TRUE)
										{
										}
												else
												{
													m_HHSound.Play(g_pDS[2], true);
													p3musicstart = TRUE;
												}
												//배경
													::__PutImage(g_pDDSBG3, Bx, 0);
												if (Timer[11] > 0)
												{
													Timer[11] -= 1;
												}
												if (Timer[12] > 0)
												{
													Timer[12] -= 1;
												}
												//이동 및 점프 로직
												///////////////////////
												///////////////////////
												///////////////////////
												///////////////////////
												//				g_BPos.x += 1;

												if (g_BPos.x + 60 < g_Pos.x) //보스 오른쪽 전환
												{
													g_bBLR = TRUE;
													g_bBLL = FALSE;
												}
												else if (g_BPos.x > g_Pos.x - 60) //보스 왼쪽 전환
												{
													g_bBLR = FALSE;
													g_bBLL = TRUE;
												}
												if (g_BPos.x + 300 < g_Pos.x) //보스 오른쪽 이동
												{
													if (BossAtt == FALSE && g_bGatt == FALSE)
													{
														g_BPos.x += 1;
													}
													g_bBLR = TRUE;
													g_bBLL = FALSE;
												}
												else if (g_BPos.x - 200 > g_Pos.x) //보스 왼쪽 이동
												{
													if (BossAtt == FALSE && g_bGatt == FALSE)
													{
														g_BPos.x -= 1;
													}
													g_bBLR = FALSE;
													g_bBLL = TRUE;
												}
												if (::GetKeyState(0x58) & 0x80 && g_bAtt == FALSE && g_bRush == FALSE) //공격 로직
												{
													if (g_bLookR == TRUE)
													{
														g_bAR = TRUE;
														g_bAtt = TRUE;
														Aframe = 12;
														if (g_pDS[3])
															m_HHSound.Stop(g_pDS[3]);
														m_HHSound.Play(g_pDS[3], false);
													}
													else if (g_bLookL == TRUE)
													{
														g_bAL = TRUE;
														g_bAtt = TRUE;
														Aframe = 0;
														if (g_pDS[3])
															m_HHSound.Stop(g_pDS[3]);
														m_HHSound.Play(g_pDS[3], false);
													}
												}
												//이동 및 점프 로직 끝
												//// 표면 클리어	
												//_FillSurface(g_pDDSBack, 0x00000000);
												nX = g_Pos.x;
												nY = g_Pos.y;
												bnX = g_BPos.x;
												bnY = g_BPos.y;
												SnX = nX;
												SnY = nY - 65;
												//플레이어 피격렉트 설정
												if (g_bLookL)
												{
													PHIT = { nX + 11,nY + 10,nX + 45,nY + 80 };
												}
												else if (g_bLookR)
												{
													PHIT = { nX + 25,nY + 10,nX + 58,nY + 80 };
												}
												//// 보스 스탠딩
												if (Timer[20] == 0 && BossAtt == FALSE && g_bGatt == FALSE && g_bSpike == FALSE)
												{
													Timer[21] = 4;
													g_bGatt = TRUE;
													if (g_bBLL == TRUE)
													{
														GAframe = 0;
														BALL = TRUE;
														BARR = FALSE;
													}
													else { GAframe = 30; BARR = TRUE; BALL = FALSE; }
												}
												else if (Timer[20] > 0)
												{
													Timer[20] -= 1;
												}
												if (g_bGatt == TRUE)
												{
													if (BALL == TRUE)
													{
														::__PutAnimeSprite(g_pDDSBoss3Att3, bnX - 503, bnY - 515, 31, GAframe, 1137, 798, 0);
													}
													else if (BARR == TRUE)
													{
														::__PutAnimeSprite(g_pDDSBoss3AttR3, bnX - 457, bnY - 515, 31, GAframe, 1137, 798, 0);
													}
													if (Timer[21] > 0)
													{
														Timer[21] -= 1;
													}
													else if (Timer[21] == 0)
													{
														if (GAframe == 18 && BALL == TRUE)
														{
															Timer[22] = 30;
															g_bSpike = TRUE;
															P3randpat = rand () % 3;
															if (P3randpat == P3randpat2)
															{
																if (P3randpat == 0)
																{
																	P3randpat = 1;
																}
																else if (P3randpat == 1)
																{
																	P3randpat = 2;
																}
																else if (P3randpat == 2)
																{
																	P3randpat = 0;
																}
															}
															if (P3randpat == 0)
															{
																SpikeX[0] = nX - 500;
																SpikeX[1] = nX - 400;
																SpikeX[2] = nX - 200;
																SpikeX[3] = nX - 50;
																SpikeX[4] = nX;
																SpikeX[5] = nX + 50;
																SpikeX[6] = nX + 200;
																SpikeX[7] = nX + 400;
																SpikeX[8] = nX + 500;
															}
															if (P3randpat == 1)
															{
																SpikeX[0] = nX - 500;
																SpikeX[1] = nX - 300;
																SpikeX[2] = nX - 250;
																SpikeX[3] = nX - 200;
																SpikeX[4] = nX;
																SpikeX[5] = nX + 200;
																SpikeX[6] = nX + 250;
																SpikeX[7] = nX + 300;
																SpikeX[8] = nX + 500;
															}
															if (P3randpat == 2)
															{
																SpikeX[0] = nX - 400;
																SpikeX[1] = nX - 200;
																SpikeX[2] = nX - 100;
																SpikeX[3] = nX - 50;
																SpikeX[4] = nX;
																SpikeX[5] = nX + 50;
																SpikeX[6] = nX + 100;
																SpikeX[7] = nX + 200;
																SpikeX[8] = nX + 400;
															}
															for (i = 0; i < 9; i++)
															{
																Spike[i] = { SpikeX[i]+ 10,380,SpikeX[i]+75,690 };
															}
															SpikeY = 300;
														}
														if (GAframe == 14 && BARR == TRUE)
														{
															Timer[22] = 50;
															g_bSpike = TRUE;
															P3randpat = rand() % 3;
															if (P3randpat == P3randpat2)
															{
																if (P3randpat == 0)
																{
																	P3randpat = 1;
																}
																else if (P3randpat == 1)
																{
																	P3randpat = 2;
																}
																else if (P3randpat == 2)
																{
																	P3randpat = 0;
																}
															}
															if (P3randpat == 0)
															{
																SpikeX[0] = nX - 500;
																SpikeX[1] = nX - 400;
																SpikeX[2] = nX - 200;
																SpikeX[3] = nX - 50;
																SpikeX[4] = nX;
																SpikeX[5] = nX + 50;
																SpikeX[6] = nX + 200;
																SpikeX[7] = nX + 400;
																SpikeX[8] = nX + 500;
															}
															if (P3randpat == 1)
															{
																SpikeX[0] = nX - 500;
																SpikeX[1] = nX - 300;
																SpikeX[2] = nX - 250;
																SpikeX[3] = nX - 200;
																SpikeX[4] = nX;
																SpikeX[5] = nX + 200;
																SpikeX[6] = nX + 250;
																SpikeX[7] = nX + 300;
																SpikeX[8] = nX + 500;
															}
															if (P3randpat == 2)
															{
																SpikeX[0] = nX - 400;
																SpikeX[1] = nX - 200;
																SpikeX[2] = nX - 100;
																SpikeX[3] = nX - 50;
																SpikeX[4] = nX;
																SpikeX[5] = nX + 50;
																SpikeX[6] = nX + 100;
																SpikeX[7] = nX + 200;
																SpikeX[8] = nX + 400;
															}
															for (i = 0; i < 9; i++)
															{
																Spike[i] = { SpikeX[i] + 10,380,SpikeX[i] + 75,690 };
															}
															SpikeY = 300;
														}
														if (BALL == TRUE)
														{
															GAframe++;
														}
														else if (BARR == TRUE)
														{
															GAframe--;
														}
														if (GAframe < 16 && BALL == TRUE)
														{
															Timer[21] = 4;
														}
														else if (GAframe > 16 && BALL == TRUE)
														{
															Timer[21] = 3;
														}
														if (GAframe >16 && BARR == TRUE)
														{
															Timer[21] = 4;
														}
														else if (GAframe < 16 && BARR == TRUE)
														{
															Timer[21] = 3;
														}
													}
													if (GAframe == 30 && BALL == TRUE)
													{
														Timer[20] = (rand() % 450) + 300;
														if (P3HP < 1000)
														{
															Timer[20] -= 250;
														}
														Timer[21] = 0;
														g_bGatt = FALSE;
														BALL = FALSE;
														BARR = FALSE;
														GAframe = 0;
														if (P3randpat == 0)
														{
															P3randpat2 = 0;
														}
														else if (P3randpat == 1)
														{
															P3randpat2 = 1;
														}
														else if (P3randpat == 2)
														{
															P3randpat2 = 2;
														}
													}
													if (GAframe == 0 && BARR == TRUE)
													{
														Timer[20] = (rand() % 450) + 300;
														if (P3HP < 1000)
														{
															Timer[20] -= 250;
														}
														Timer[21] = 0;
														g_bGatt = FALSE;
														BALL = FALSE;
														BARR = FALSE;
														GAframe = 0;
														if (P3randpat == 0)
														{
															P3randpat2 = 0;
														}
														else if (P3randpat == 1)
														{
															P3randpat2 = 1;
														}
														else if (P3randpat == 2)
														{
															P3randpat2 = 2;
														}
													}
												}
												if (Timer[22] > 0 && g_bSpike == TRUE)
												{
													Timer[22] -= 1;
													for (i = 0; i < 9; i++)
													{
														::__PutSprite(g_pDDSDHole,SpikeX[i], 600);
													}
												}
												else if (Timer[22] == 0 && g_bSpike == TRUE)
												{
													Timer[23] = 3;
													Timer[22] = -1;
													SPframe = 0;
												}
												if (Timer[22] == -1 && g_bSpike == TRUE & Timer[23] > 0)
												{
													Timer[23] -= 1;
													if (SPframe == 15)
													{
														Timer[22] = 0;
														g_bSpike = FALSE;
														Timer[23] = 0;
														SPframe = 0;
													}
													if (g_bSpike == TRUE)
													{
														for (i = 0; i < 9; i++)
														{
															::__PutAnimeSprite(g_pDDSGroundAtt, SpikeX[i] - 130, 40, 15, SPframe, 422, 629, 0);
														}
														if (SPframe > 1 && SPframe <= 4)
														{
															for (i = 0; i < 88; i++)
															{
																if (IntersectRect(&rcDest, &Spike[i], &PHIT))
																{
																	PlayerHP -= 1000;
																	Spike[i] = { 0,0,1,1 };
																	if (PHplay == FALSE)
																	{
																		PHplay = TRUE;
																		Timer[25] = 3;
																		PHframe = 0;
																	}
																	else if (PHplay == TRUE)
																	{
																		PHplay = TRUE;
																		Timer[25] = 3;
																		PHframe = 0;
																	}
																}
															}
														}
													}
												}
												if (Timer[23] == 0 && g_bSpike ==TRUE && Timer[22] == -1) 
												{
													SPframe += 1;
													Timer[23] = 3;
												}
												if (Timer[17] == 0 && BossAtt == FALSE && g_bGatt == FALSE)
												{
													Timer[16] = 4;
													BossAtt = TRUE;
													if (g_bBLL == TRUE)
													{
														BKframe = 0;
														BAL = TRUE;
														BAR = FALSE;
													}
													else { BKframe = 23; BAR = TRUE; BAL = FALSE; }
												}
												else if (Timer[17] > 0)
												{
													Timer[17] -= 1;
												}
												if (BossAtt == TRUE)
												{
													if (BAL == TRUE)
													{
														::__PutAnimeSprite(g_pDDSBoss3Att, bnX - 263, bnY - 151, 24, BKframe, 670, 384, 0);
													}
													else if (BAR == TRUE)
													{
														::__PutAnimeSprite(g_pDDSBoss3AttR, bnX - 257, bnY - 151, 24, BKframe, 670, 384, 0);
													}
													if (Timer[16] > 0)
													{
														Timer[16] -= 1;
													}
													else if (Timer[16] == 0)
													{
														if (BAL == TRUE)
														{
															BKframe++;
														}
														else if (BAR == TRUE)
														{
															BKframe--;
														}
														if (BKframe < 12 && BAL == TRUE)
														{
															Timer[16] = 4;
														}
														else if (BKframe > 12 && BAL == TRUE)
														{
															Timer[16] = 3;
														}
														if (BKframe >11 && BAR == TRUE)
														{
															Timer[16] = 4;
														}
														else if (BKframe < 11 && BAR == TRUE)
														{
															Timer[16] = 3;
														}
													}
													if (BKframe == 23 && BAL == TRUE)
													{
														Timer[17] = (rand() % 150) + 200;
														if (P3HP < 1000)
														{
															Timer[17] -= 50;
														}
														Timer[16] = 0;
														BossAtt = FALSE;
														BAL = FALSE;
														BAR = FALSE;
														KnockHIT = FALSE;
														BKframe = 0;
														Knock = { 0,0,1,1 };
													}
													if (BKframe == 0 && BAR == TRUE)
													{
														Timer[17] = (rand() % 150) + 200;
														if (P3HP < 1000)
														{
															Timer[17] -= 50;
														}
														Timer[16] = 0;
														BossAtt = FALSE;
														BAL = FALSE;
														BAR = FALSE;
														BKframe = 0;
														KnockHIT = FALSE;
														Knock = { 0,0,1,1 };
													}
												}
												if (BKframe > 13 && BAL == TRUE && KnockHIT == FALSE)
												{
													Knock = { bnX - 230 ,bnY - 20,bnX + 80,bnY + 140 };
												}
												if (BKframe <11 && BAR == TRUE && KnockHIT == FALSE)
												{
													Knock = { bnX + 90 ,bnY - 20,bnX + 393,bnY + 140 };
												}
												if (BKframe > 15 && BAL == TRUE)
												{
													Knock = { 0,0,1,1 };
												}
												if (BKframe <9 && BAR == TRUE)
												{
													Knock = { 0,0,1,1 };
												}
												if (IntersectRect(&rcDest, &Knock, &PHIT))
												{
													if (BAR == TRUE && Knockback == FALSE)
													{
														Knockspd = 30;
														Knockback = TRUE;
														PlayerHP -= 1000;
														KnockHIT = TRUE;
														Knock = { 0,0,1,1 };
														if (PHplay == FALSE)
														{
															PHplay = TRUE;
															Timer[25] = 3;
															PHframe = 0;
														}
														else if (PHplay == TRUE)
														{
															PHplay = TRUE;
															Timer[25] = 3;
															PHframe = 0;
														}
														if (g_bRush == TRUE)
														{
															g_bRush = FALSE;
															g_bRushL = FALSE;
															g_bRushR = FALSE;
															Timer[9] = 20;
															Timer[8] = 0;
															Cframe = 0;
														}

														if (g_bJump == TRUE)
														{
															Knockspd = 35;
														}
													}
													else if (BAL == TRUE && Knockback == FALSE)
													{
														Knockspd = -30;
														Knockback = TRUE;
														KnockHIT = TRUE;
														PlayerHP -= 1000;
														Knock = { 0,0,1,1 };
														if (PHplay == FALSE)
														{
															PHplay = TRUE;
															Timer[25] = 3;
															PHframe = 0;
														}
														else if (PHplay == TRUE)
														{
															PHplay = TRUE;
															Timer[25] = 3;
															PHframe = 0;
														}
														if (g_bRush == TRUE)
														{
															g_bRush = FALSE;
															g_bRushL = FALSE;
															g_bRushR = FALSE;
															Timer[9] = 20;
															Timer[8] = 0;
															Cframe = 0;
														}
														if (g_bJump == TRUE)
														{
															Knockspd = -35;
														}
													}
												}
												if (p3die == FALSE && BossAtt == FALSE && g_bGatt == FALSE)
												{
													BOSS = { bnX + 10 ,bnY,bnX + 170,bnY + 148 };
													if (Timer[1] >= 8)
													{
														Timer[1] = 0;
														Bframe++;
													}
													else { Timer[1]++; }
													if (Bframe >= 15)
													{
														Bframe = 0;
													}
													if (g_bBLL == TRUE)
													{
														::__PutAnimeSprite(g_pDDSP3standL, bnX-30, bnY-100, 16, Bframe, 243, 314, 0);
													}
													else if (g_bBLR == TRUE)
													{
														::__PutAnimeSprite(g_pDDSP3standR, bnX-30, bnY-100, 16, Bframe, 243, 314, 0);
													}
												}
												if (Timer[0] >= 26)
												{
													Timer[0] = 0;
												}
												//캐릭터 돌진 애니메이션
												if (g_bRush == FALSE)
												{
													//스탠딩
													if (g_bLookL == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
													}
													else if (g_bLookR == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
													}
													//점프
													if (g_bLookL == 1 && g_bJump == 1 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 3, 86, 81, 0);
													}
													else if (g_bLookR == 1 && g_bJump == 1 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 5, 86, 81, 0);
													}
													//무빙
													if (g_bLeft == 1 && g_bJump == 0 && Timer[0] < 5 && g_bJump == 0 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 1, 86, 81, 0);
													}
													else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 5 && Timer[0] < 12 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
													}
													else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 12 && Timer[0] < 19 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 2, 86, 81, 0);
													}
													else if (g_bLeft == 1 && g_bJump == 0 && Timer[0] >= 19 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
													}
													if (g_bRight == 1 && g_bJump == 0 && Timer[0] < 5 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 7, 86, 81, 0);
													}
													else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 5 && Timer[0] < 12 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
													}
													else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 12 && Timer[0] < 19 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 6, 86, 81, 0);
													}
													else if (g_bRight == 1 && g_bJump == 0 && Timer[0] >= 19 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
													}
													if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] < 7)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 1, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 7 && Timer[4] < 14)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 2, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 14 && Timer[4] < 21)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 3, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 21 && Timer[4] < 27)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 4, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAR == TRUE && Timer[4] >= 27 && Timer[4] < 39)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 0, 86, 81, 0);
													}
													if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] < 7)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 4, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 7 && Timer[4] < 14)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 5, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 14 && Timer[4] < 21)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 6, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 21 && Timer[4] < 27)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 7, 86, 81, 0);
													}
													else if (g_bAtt == TRUE && g_bAL == TRUE && Timer[4] >= 27 && Timer[4] < 39)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 8, 86, 81, 0);
													}
													if (g_bAtt == TRUE && Timer[4] < 38)
													{
														Timer[4]++;
													}
													else
													{
														Timer[4] = 0;
													}
												}
												if (g_bRushL == TRUE)
												{
													::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 7, 86, 81, 0);
													::__PutAnimeSprite(g_pDDSCHARGE, nX - 170, nY - 100, 8, Cframe, 511, 263, 0);
												}
												else if (g_bRushR == TRUE)
												{
													::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 1, 86, 81, 0);
													::__PutAnimeSprite(g_pDDSCHARGER, nX - 250, nY - 100, 8, Cframe, 511, 263, 0);
												}
												if (g_bAtt == TRUE)
												{
													if (g_bAL == TRUE)
													{
														if (g_bHit == FALSE)
														{
															HIT = { SnX - 130,SnY + 10,SnX + 90,SnY + 180 };
														}
														else
														{
															HIT = { 0, 0, 1, 1 };
														}
														if (Timer[2] < 2)
														{
															Timer[2]++;
														}
														else
														{
															Timer[2] = 0;
															if (Aframe < 12)
															{
																Aframe++;
															}
															else { Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE; }
														}
														::__PutAnimeSprite(g_pDDSSkill, SnX - 140, SnY, 13, Aframe, 301, 218, 0);
													}
													else if (g_bAR == TRUE)
													{
														if (g_bHit == FALSE)
														{
															HIT = { SnX - 10,SnY + 10,SnX + 220,SnY + 180 };
														}
														else
														{
															HIT = { 0, 0, 1, 1 };
														}
														if (Timer[2] < 2)
														{
															Timer[2]++;
														}
														else
														{
															Timer[2] = 0;
															if (Aframe > 0)
															{
																Aframe--;
															}
															else { Aframe == 0; g_bAL = FALSE, g_bAR = FALSE, g_bAtt = FALSE; g_bHit = FALSE; }
														}
														::__PutAnimeSprite(g_pDDSSkillR, SnX - 70, SnY, 13, Aframe, 301, 218, 0);
													}
												}
												if (bossimmu == FALSE && g_bHit == FALSE)
												{
													if (IntersectRect(&rcDest, &BOSS, &HIT))
													{
														P3HP -= 100;
														bossimmu = TRUE;
														Timer[5] = 20;
														g_bHit = TRUE;
														if (BHMPlay == FALSE)
														{
															BHMPlay = TRUE;
															BHplay[0] = TRUE;
															Timer[24] = 2;
														}
														else if (BHMPlay == TRUE)
														{
															BHplay[0] = TRUE;
															BHplay[1] = FALSE;
															BHplay[2] = FALSE;
															BHframe[0] = 0;
															BHframe[1] = 0;
															BHframe[2] = 0;
															Timer[24] = 2;
															Timer[26] = 2;
															Timer[27] = 2;
														}
													}
												}
												if (g_bRight == 1 || g_bLeft == 1 && g_bJump == 0)
												{
													Timer[0] += 1;
												}
												else
												{
													Timer[0] = 0;
												}
												if (p3die == 0)
												{
													if (Timer[7] == 0)
													{
														if (g_bBAT == TRUE)
														{
															if (BAframe < 10 && Timer[6] < 3)
															{
																BAHIT[0] = { 35,200,115,700 };
																BAHIT[1] = { 255,200,335,700 };
																BAHIT[2] = { 475,200,555,700 };
																BAHIT[3] = { 695,200,775,700 };
																BAHIT[4] = { 915,200,995,700 };
																Timer[6]++;
															}
															else if (BAframe < 10 && Timer[6] >= 3)
															{
																BAframe++;
																Timer[6] = 0;
															}
															if (BAframe >= 13)
															{
																int i = 0;
																for (i = 0; i < 5; i++)
																{
																	if (RandNum[i] == FALSE)
																	{
																		if (IntersectRect(&rcDest, &BAHIT[i], &PHIT))
																		{
																			PlayerHP -= 500;
																			BAHIT[i] = { 0,0,1,1 };
																			if (PHplay == FALSE)
																			{
																				PHplay = TRUE;
																				Timer[25] = 3;
																				PHframe = 0;
																			}
																			else if (PHplay == TRUE)
																			{
																				PHplay = TRUE;
																				Timer[25] = 3;
																				PHframe = 0;
																			}
																		}
																	}
																}
															}
															if (BAframe >= 10 && Timer[6] < 2 && BAframe < 19)
															{
																Timer[6]++;
															}
															else if (BAframe >= 10 && Timer[6] >= 2 && BAframe < 19)
															{
																BAframe++;
																Timer[6] = 0;
															}
															switch (Randpat)
															{
															case 0:
																::__PutAnimeSprite(g_BossAtt, -170, 50, 20, BAframe, 600, 656, 0);
																::__PutAnimeSprite(g_BossAtt, 270, 50, 20, BAframe, 600, 656, 0);
																::__PutAnimeSprite(g_BossAtt, 710, 50, 20, BAframe, 600, 656, 0);
																break;
															case 1:
																::__PutAnimeSprite(g_BossAtt, 50, 50, 20, BAframe, 600, 656, 0);
																::__PutAnimeSprite(g_BossAtt, 490, 50, 20, BAframe, 600, 656, 0);
																break;
															case 2:
																if (RandNum[0] == TRUE)
																{
																	::__PutAnimeSprite(g_BossAtt2, -170, 50, 20, BAframe, 600, 656, 0);
																}
																else
																{
																	::__PutAnimeSprite(g_BossAtt, -170, 50, 20, BAframe, 600, 656, 0);
																}
																if (RandNum[1] == TRUE)
																{
																	::__PutAnimeSprite(g_BossAtt2, 50, 50, 20, BAframe, 600, 656, 0);
																}
																else
																{
																	::__PutAnimeSprite(g_BossAtt, 50, 50, 20, BAframe, 600, 656, 0);
																}
																if (RandNum[2] == TRUE)
																{
																	::__PutAnimeSprite(g_BossAtt2, 270, 50, 20, BAframe, 600, 656, 0);
																}
																else
																{
																	::__PutAnimeSprite(g_BossAtt, 270, 50, 20, BAframe, 600, 656, 0);
																}
																if (RandNum[3] == TRUE)
																{
																	::__PutAnimeSprite(g_BossAtt2, 490, 50, 20, BAframe, 600, 656, 0);
																}
																else
																{
																	::__PutAnimeSprite(g_BossAtt, 490, 50, 20, BAframe, 600, 656, 0);
																}
																if (RandNum[4] == TRUE)
																{
																	::__PutAnimeSprite(g_BossAtt2, 710, 50, 20, BAframe, 600, 656, 0);
																}
																else
																{
																	::__PutAnimeSprite(g_BossAtt, 710, 50, 20, BAframe, 600, 656, 0);
																}
																break;
															}
															if (BAframe >= 19)
															{
																if (Timer[6] > 2)
																{
																	BAframe = 0;
																	Randpat2 = Randpat;
																	g_bBAT = FALSE;
																	Timer[7] = 60;
																	if (P3HP < 1000)
																	{
																		Timer[7] -= 55;
																	}
																	Maximum = 0;
																	RandNum[0, 1, 2, 3, 4] = { 0 };
																}
																else
																{
																	Timer[6]++;
																}
															}
														}
														else
														{
															Randpat = rand() % 3;
															if (Randpat == Randpat2)
															{
																switch (Randpat)
																{
																case 0:
																	Randpat = 1;
																	break;
																case 1:
																	Randpat = 2;
																	break;
																case 2:
																	Randpat = 0;
																	break;
																}
															}
															if (Randpat == 2)
															{
																RandNum[0] = rand() % 2;
																if (RandNum[0] == 1 && Maximum < 2)
																{
																	Maximum++;
																}
																else if (RandNum[0] == 1 && Maximum >= 2)
																{
																	RandNum[0] = 0;
																}
																RandNum[1] = rand() % 2;
																if (RandNum[1] == 1 && Maximum < 2)
																{
																	Maximum++;
																}
																else if (RandNum[1] == 1 && Maximum >= 2)
																{
																	RandNum[1] = 0;
																}
																RandNum[2] = rand() % 2;
																if (RandNum[2] == 1 && Maximum < 2)
																{
																	Maximum++;
																}
																else if (RandNum[2] == 1 && Maximum >= 2)
																{
																	RandNum[2] = 0;
																}
																RandNum[3] = rand() % 2;
																if (RandNum[3] == 1 && Maximum < 2)
																{
																	Maximum++;
																}
																else if (RandNum[3] == 1 && Maximum >= 2)
																{
																	RandNum[3] = 0;
																}
																RandNum[4] = rand() % 2;
																if (RandNum[4] == 1 && Maximum < 2)
																{
																	Maximum++;
																}
																else if (RandNum[4] == 1 && Maximum >= 2)
																{
																	RandNum[4] = 0;
																}
																if (Maximum == 0)
																{
																	int n = 0;
																	n = rand() % 4;
																	RandNum[n] = 1;
																}
															}
															switch (Randpat)
															{
															case 0:
																RandNum[0] = 0;
																RandNum[1] = 1;
																RandNum[2] = 0;
																RandNum[3] = 1;
																RandNum[4] = 0;
																break;
															case 1:
																RandNum[0] = 1;
																RandNum[1] = 0;
																RandNum[2] = 1;
																RandNum[3] = 0;
																RandNum[4] = 1;
																break;
															}
															g_bBAT = TRUE;
														}
													}
												}
												if (PHplay == TRUE)
												{
													if (Timer[25] == 0)
													{
														PHframe++;
														Timer[25] = 3;
														if (PHframe > 5)
														{
															PHplay = FALSE;
															PHframe = 0;
															Timer[25] = 0;
														}
													}
													else { Timer[25] --; }
													if (PHplay == TRUE)
													{
														if (g_bLookL)
														{
															::__PutAnimeSprite(g_pDDSDPlayerHit, nX - 50, nY - 70, 6, PHframe, 180, 226, 0);
														}
														if (g_bLookR)
														{
															::__PutAnimeSprite(g_pDDSDPlayerHit, nX - 40, nY - 70, 6, PHframe, 180, 226, 0);
														}
													}
												}
												if (BHMPlay == TRUE)
												{
													if (Timer[24] == 0 && BHplay[0] == TRUE)
													{
														BHframe[0]++;
														Timer[24] = 2;
														if (BHframe[0] == 6)
														{
															BHframe[0] = 0;
															Timer[24] = 0;
															BHplay[0] = FALSE;
														}
														if (BHframe[0] == 4 && BHplay[1] == FALSE)
														{
															Timer[26] = 2;
															BHplay[1] = TRUE;
														}
													}
													else if (Timer[24] > 0 && BHplay[0] == TRUE) { Timer[24]--; }
													if (Timer[26] == 0 && BHplay[1] == TRUE)
													{
														BHframe[1]++;
														Timer[26] = 2;
														if (BHframe[1] == 6)
														{
															BHframe[1] = 0;
															Timer[26] = 0;
															BHplay[1] = FALSE;
														}
														if (BHframe[1] == 4 && BHplay[2] == FALSE)
														{
															Timer[27] = 2;
															BHplay[2] = TRUE;
														}
													}
													else if (Timer[26] > 0 && BHplay[1] == TRUE) { Timer[26]--; }
													if (Timer[27] == 0 && BHplay[2] == TRUE)
													{
														BHframe[2]++;
														Timer[27] = 2;
														if (BHframe[2] == 6)
														{
															BHframe[2] = 0;
															Timer[27] = 0;
															BHplay[2] = FALSE;
														}
													}
													else if (Timer[27] > 0 && BHplay[2] == TRUE) { Timer[27]--; }
													if (BHplay[0] == FALSE && BHplay[1] == FALSE && BHplay[2] == FALSE)
													{
														BHMPlay = FALSE;
													}
													if (BHplay[0] == TRUE)
													{
														if (g_bLookL)
														{
															::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[0], 180, 226, 0);
														}
														if (g_bLookR)
														{
															::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[0], 180, 226, 0);
														}
													}
													if (BHplay[1] == TRUE)
													{
														if (g_bLookL)
														{
															::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[1], 180, 226, 0);
														}
														if (g_bLookR)
														{
															::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[1], 180, 226, 0);
														}
													}
													if (BHplay[2] == TRUE)
													{
														if (g_bLookL)
														{
															::__PutAnimeSprite(g_pDDSDBossHit, bnX, bnY - 40, 6, BHframe[2], 180, 226, 0);
														}
														if (g_bLookR)
														{
															::__PutAnimeSprite(g_pDDSDBossHit, bnX + 20, bnY - 40, 6, BHframe[2], 180, 226, 0);
														}
													}
												}
												if (P3HP < 1)
												{
													if (g_pDS[0])
														m_HHSound.Stop(g_pDS[0]);
													p3die = 1;
													g_nPhaze = 4;
													g_Pos.x = 0;
													g_Pos.y = 0;
													g_BPos.x = 0;
													g_BPos.y = 0;
													Gravity = 0;
													g_bSetSpeed = FALSE;
													g_bJump = FALSE;
													g_bJaccel = FALSE;
													g_bJLeft = FALSE;
													g_bJRight = FALSE;
													g_bCast = FALSE;
													Timer[17] = 0;
													Timer[16] = 0;
													BossAtt = FALSE;
													BAL = FALSE;
													BAR = FALSE;
													BKframe = 0;
													KnockHIT = FALSE;
													Knock = { 0,0,1,1 };
													BHplay[0] = FALSE;
													BHplay[1] = FALSE;
													BHplay[2] = FALSE;
													BAframe = 0;
													Randpat2 = Randpat;
													g_bBAT = FALSE;
													Timer[7] = 60;
													Maximum = 0;
													RandNum[0, 1, 2, 3, 4] = { 0 };
													BHMPlay = FALSE;
													BHframe[0] = 0;
													BHframe[1] = 0;
													BHframe[2] = 0;
													Timer[24] = 2;
													Timer[26] = 2;
													Timer[27] = 2;

												}
												//렉트 확인용
												/*
												::__Lock(g_pDDSBack);
												__DrawRect16(PHIT.left, PHIT.top, PHIT.right, PHIT.bottom, wPixel);
												__DrawRect16(HIT.left, HIT.top, HIT.right, HIT.bottom, wPixel);
												__DrawRect16(Cast.left, Cast.top, Cast.right, Cast.bottom, wPixel);
												__DrawRect16(BOSS.left, BOSS.top, BOSS.right, BOSS.bottom, wPixel);
												__DrawRect16(Knock.left, Knock.top, Knock.right, Knock.bottom, wPixel);
												for (i = 0; i < 5; i++)
												{
													__DrawRect16(BAHIT[i].left, BAHIT[i].top, BAHIT[i].right, BAHIT[i].bottom, wPixel);
													__DrawRect16(Spike[i].left, Spike[i].top, Spike[i].right, Spike[i].bottom, wPixel);
												}
												::__Unlock(g_pDDSBack);
												*/
												if (Timer[7] > 0)
												{
													Timer[7]--;
												}
												if (Timer[5] > 0)
												{
													Timer[5] -= 1;
												}
												else if (Timer[5] <= 0 && bossimmu == TRUE)
												{
													bossimmu = FALSE;
													Timer[5] = 0;
												}
												::__PutSprite(g_pDDSHP, 130, 0);
												if (P3HP > 1)
												{
													::__Putbar(g_pDDSHPbar, 132, 2, P3MHP, P3HP, P3HPrate);
												}
												::__PutSprite(g_pDDSPHP, 400, 737);
												if (PlayerHP > 1)
												{
													::__Putbar(g_pDDSPHPbar, 425, 744, PlayerMHP, PlayerHP, PlayerHPrate);
												}
												//////////////////////////////////////////////////////////
												///// 기타정보 
												DXW_DisplayInfo();
												break;
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												//////////////////////////////////////////////////////////
												case 4:
													if (P4Mus == FALSE)
													{
														m_HHSound.Stop(g_pDS[2]);
														P4Mus = TRUE;
													}
													__PutImage(g_pDDSEnd, 0, 0);
													break;

												case 5:
													if (P5Mus == FALSE)
													{
														m_HHSound.Stop(g_pDS[0]);
														m_HHSound.Stop(g_pDS[1]);
														m_HHSound.Stop(g_pDS[2]);
														P5Mus = TRUE;
													}
													__PutImage(g_pDDSGameover, 0, 0);
													break;

	}
	if (g_nPhaze > 0 && g_nPhaze < 4)
	{
		::__PutSprite(g_pDDSSkillBut, 250, 720);
	}
	if (Timer[28] > 10 && g_nPhaze > 0 && g_nPhaze < 4)
	{
		__Putbar(g_pDDSSkillCool, 251, 721, MaxTime, Timer[28], Timerrate);
	}
	if (Skilluse == TRUE && g_nPhaze > 0 && g_nPhaze < 4)
	{
		::__PutAnimeSprite(g_pDDSPlayerskill, 0, 0, 20, PSframe, 1024, 768, 0);
	}
	if (PlayerHP < 1 && g_nPhaze < 4)
	{
		g_nPhaze = 5;
	}
		if (g_bFadeIn)
		{
			__FxFadeInOutEx(g_pDDSBack, g_pDDSBack, g_dwFadeRate);
			g_dwFadeRate += 2;
			if (g_dwFadeRate > 99) g_bFadeIn = FALSE;
		}

		if (g_bFadeOut)
		{
			__FxFadeInOutEx(g_pDDSBack, g_pDDSBack, g_dwFadeRate);
			g_dwFadeRate -= 2;
			if (g_dwFadeRate < 1) g_bFadeOut = FALSE;
		}

		if (g_bFadeLoop)
		{
			__FxFadeInOutEx(g_pDDSBack, g_pDDSBack, g_dwFadeRate);
			if (g_bInc)
				g_dwFadeRate += 2;
			else
				g_dwFadeRate -= 2;

			if (g_dwFadeRate <= 0) { g_bInc = TRUE;  g_dwFadeRate = 0; }
			if (g_dwFadeRate > 100) { g_bInc = FALSE; g_dwFadeRate = 100; }
		}

		if (g_bGrayScale) __FxGrayScale(g_pDDSBack, g_pDDSBack);

		if (g_bColorize) __FxColorize(g_pDDSBack, g_pDDSBack, g_dwR, g_dwG, g_dwB);
	///// 플립핑 또는 버퍼링
#ifdef _DEBUG
	hRet = __DDSWBlt();
#elif _NOVSYNC
	hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_NOVSYNC);
#else
	hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_WAIT);
#endif

	///// 표면복구	
	if (hRet == DDERR_SURFACELOST) g_pDD->RestoreAllSurfaces();
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }

	///// 프레임 조정
	gFpsMng.FrameTuningEx();

}

void DXW_DisplayInfo()
{
	///// 프레임수 측정 및 기타 정보
    if (gAppSet.bIsDisplayInfo)
    {
		DWORD dwFps = gFpsMng.GetFPS();

		/////
		__GetDC(g_pDDSBack);
		{
			__PutTextf(10, 30, PEN, "P3PAT %d/%d", P3randpat,P3randpat2);
			__PutTextf(10, 10, PEN, "FPS %d", dwFps);
			__PutTextf(465, 741, PEN, "%d/%d", PlayerHP, PlayerMHP);
		}
		__ReleaseDC(g_pDDSBack);
	}
}

void __LockMemcpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc)
{
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

    //// 소스표면의 RECT
    ddsdSrc.dwSize = sizeof(ddsdSrc);  	
	lpDDSSrc->GetSurfaceDesc(&ddsdSrc);
    RECT rcSrc = { 0, 0, ddsdSrc.dwWidth, 1 };
	DWORD dwWidth = ddsdSrc.dwWidth;	
	
	//// 소스표면 Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDSSrc->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }

    //// 목적표면의 RECT
    ddsdDst.dwSize = sizeof(ddsdDst);    	
	lpDDSDst->GetSurfaceDesc(&ddsdDst);
    RECT rcDst = { 0, 0, ddsdDst.dwWidth, 1 };        

	//// 목적표면 Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = lpDDSDst->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// 소스표면은 Lock 상태이므로 Unlock
        lpDDSSrc->Unlock(&rcSrc);
        DDERRCHK(ddrval);     
        return;
    }

	//// 표면의 주소
	LPBYTE lpSrc = (LPBYTE) ddsdSrc.lpSurface;
	LPBYTE lpDst = (LPBYTE) ddsdDst.lpSurface;

	//TRACE2("Src Addr - %x\n Dst Addr - %x\n", lpSrc, lpDst);
	::memcpy(lpDst, lpSrc, dwWidth);

    lpDDSDst->Unlock(&rcDst);
    lpDDSSrc->Unlock(&rcSrc);

	//Sleep(1);
}

void __BltCpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc)
{	
	/////	
	HRESULT ddrval = lpDDSDst->Blt(NULL, lpDDSSrc, NULL, DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

///////////////////////////////////////////////////////////////////////////////
//// 이미지데이타 표면 적재
//// 

LPDIRECTDRAWSURFACE7 __DDLoadBin(LPCSTR strFile, BOOL bIsVRAM)
{
	SIZE szBin;
	int nSize;

	//// 파일 읽기
	//// 8 바이트 : 가로, 세로크기
	//// 4 바이트 : 이미지 전체 크기
	FILE* file = fopen(strFile, "rb");
	fread(&szBin, sizeof(SIZE), 1, file);
	fread(&nSize, sizeof(int), 1, file);	
	LPBYTE lpImg = (LPBYTE) malloc(nSize);
	fread(lpImg, nSize, 1, file);
	fclose(file);

	///// 오프스크린 생성 ( VRAM )
	LPDIRECTDRAWSURFACE7 lpDDS = NULL;
	lpDDS = __CreateSurface(szBin.cx, szBin.cy, bIsVRAM);
	if (!lpDDS) { MSGERROR("_DDLoadBin"); return NULL; }

	///// 생성된 오프스크린에 메모리 데이타 전송
	BOOL bRval = __MEM2DDS(lpDDS, lpImg, szBin.cx, szBin.cy);
	if (!bRval) { MSGERROR("_DDLoadBin"); return NULL; }

	free(lpImg);

	return lpDDS;
}

//// 메모리 -> 표면
//// 

BOOL __MEM2DDS(LPDIRECTDRAWSURFACE7 lpDstDDS, LPBYTE lpSrcMem, int nWidth, int nHeight)
{
	int iy;
	DDSURFACEDESC2 ddsd;
	HRESULT hRet;
		
	int nWidthStep=0;

	//// 비트카운트에 따라 계산된 가로폭을 구한다
	switch (g_dwRGBBitCount)
	{
	case  8 : nWidthStep = nWidth;      break; // 
	case 16 : nWidthStep = nWidth << 1; break; // nWidth * 2
	case 24 : nWidthStep = nWidth *  3; break; // nWidth * 3
	case 32 : nWidthStep = nWidth << 2; break; // nWidth * 4
	}

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	hRet = lpDstDDS->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) return DDERRCHK(hRet);
	
	LPBYTE lpDDS = (LPBYTE) ddsd.lpSurface;

	//// 메모리 전송 ( 복사 )
	for (iy=0; iy < nHeight; iy++)
	{
		memcpy(lpDDS, lpSrcMem, nWidthStep);
		lpDDS    += ddsd.lPitch;
		lpSrcMem += nWidthStep;
	}
	
	hRet = lpDstDDS->Unlock(NULL);
	if (hRet != DD_OK) return DDERRCHK(hRet);

	return TRUE;
}

//// Blt() 이미지
////
void __PutImage(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{	
	//// 표면의 크기 정보를 얻는다 ( 임시코드 )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	/////
	RECT rcDest = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };	
	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, NULL, DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//// Blt() 스프라이트 
////
void __PutSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{	
	//// 표면의 크기 정보를 얻는다 ( 임시코드 )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	/////
	RECT rcDest = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };	
	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, NULL, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//애니메이션 출력 함수
void __PutAnimeSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y, int Count, int Number, int cutx, int cuty, int Flip)
{
	//// 표면의 크기 정보를 얻는다 ( 임시코드 )
	DDSURFACEDESC2 ddsd;
	DDBLTFX  ddbltfx;
	HRESULT ddrval;
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;

	RECT CHAR = { 0 + (cutx * Number), 0, cutx * (Number + 1), cuty };
	RECT rcDest = { x, y, x + (ddsd.dwWidth / Count), y + ddsd.dwHeight };
	//좌우반전
	if (Flip == 1)
	{
		/////
		ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, &CHAR, DDBLT_KEYSRC | DDBLT_WAIT | DDBLT_DDFX, &ddbltfx);
	}
	else if (Flip == 0)
	{
		ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, &CHAR, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	}
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void __Putbar(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y,int Mhp,int hp,float HPrate)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	HPrate = ((float)hp / (float)Mhp);
	if (HPrate <= 0)
	{
		HPrate = 0;
	}
	lpDDS->GetSurfaceDesc(&ddsd);

	/////
	RECT rcDest = { x, y, (float)x + ((float)ddsd.dwWidth * HPrate), y + ddsd.dwHeight };
	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}
//// Lock() 이미지
//// 주 의 : 목적표면(2차)이 Lock이 된 상태에서 함수 진입
//// 기 능 : 16, 32비트 모드 이미지 출력
//// lpDDS : 소스 표면
//// x, y  : 출력 좌표 

void __PutImageLock(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddsdSrc;

	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);    

	hRet = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }

	///// BPP에 따라 연산 값을 취한다
	DWORD dwMul = 1;
	switch(g_dwRGBBitCount)
	{
	case 16 : dwMul = 2; break;
	case 32 : dwMul = 4; break;
	}

	//// 이미지 표면 주소
	LPBYTE lpSrc = (LPBYTE) ddsdSrc.lpSurface;
	
	//// 2차 표면 주소 ( 전역 )	
	LPBYTE lpDst = (LPBYTE) g_ddsd.lpSurface + y*g_ddsd.lPitch+(x * dwMul);

	int nWidth  = ddsdSrc.dwWidth * dwMul;
	int nHeight = ddsdSrc.dwHeight;

	//// memcpy에 의한 메모리 전송
	for (int iy=0; iy < nHeight; iy++)
	{
		memcpy(lpDst, lpSrc, nWidth);
		lpSrc += ddsdSrc.lPitch;
		lpDst += g_ddsd.lPitch;
	}

	hRet = lpDDS->Unlock(NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
}

//// Lock() 스프라이트
//// 주 의 : 목적표면(2차)이 Lock이 된 상태에서 함수 진입
//// 기 능 : 16비트 모드 스프라이트 출력
//// lpDDS : 소스 표면
//// x, y  : 출력 좌표 

void __PutSpriteLock16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddsdSrc;

	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);    

	hRet = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
	
	//// 이미지 표면 주소
	WORD* lpSrc = (WORD*) ddsdSrc.lpSurface;
	
	//// 2차 표면 주소 ( 전역 ) , ( *2 )
	WORD* lpDst = (WORD*) ((LPBYTE)g_ddsd.lpSurface + y*g_ddsd.lPitch+(x << 1));

	int nWidth  = ddsdSrc.dwWidth;
	int nHeight = ddsdSrc.dwHeight;
	
	//// 컬러키
	DWORD dwSrcColorKey = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	WORD wSrc=0;

	//// WORD 단위로 주소가 증가 되므로 RShift ( /2)
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 1;
	DWORD dwDstPitch = g_ddsd.lPitch >> 1;

	//// 루프내에서는 최대한 연산을 최소화한다	
	for (int iy=0; iy < nHeight; iy++)
	{
		for (int ix=0; ix < nWidth; ix++)
		{			
			wSrc = *(lpSrc+ix);
			if (wSrc != dwSrcColorKey)
			{
				*(lpDst+ix) = wSrc;
			}
		}
		lpSrc += dwSrcPitch;
		lpDst += dwDstPitch;
	}

	hRet = lpDDS->Unlock(NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
}

//// Lock() 스프라이트
//// 주 의 : 목적표면(2차)이 Lock이 된 상태에서 함수 진입
//// 기 능 : 32비트 모드 스프라이트 출력
//// lpDDS : 소스 표면
//// x, y  : 출력 좌표 

void __PutSpriteLock32(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddsdSrc;

	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	ddsdSrc.dwFlags = DDSD_CKSRCBLT;

	hRet = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
	
	//// 이미지 표면 주소
	DWORD* lpSrc = (DWORD*) ddsdSrc.lpSurface;
	
	//// 2차 표면 주소 ( 전역 ) , ( *4 )
	DWORD* lpDst = (DWORD*) ((LPBYTE)g_ddsd.lpSurface + y*g_ddsd.lPitch+(x << 2));

	int nWidth  = ddsdSrc.dwWidth;
	int nHeight = ddsdSrc.dwHeight;
	
	//// 컬러키
	DWORD dwSrcColorKey = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	DWORD dwSrc=0;

	//// DWORD 단위로 주소가 증가 되므로 RShift ( /4)
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2;
	DWORD dwDstPitch = g_ddsd.lPitch >> 2;

	//// 루프내에서는 최대한 연산을 최소화한다	
	for (int iy=0; iy < nHeight; iy++)
	{
		for (int ix=0; ix < nWidth; ix++)
		{
			//// 알파 값 또는 최상위 8비트는 무시한다, ARGB
			dwSrc = *(lpSrc+ix) & 0x00FFFFFF;
			if (dwSrc != dwSrcColorKey)
			{
				*(lpDst+ix) = dwSrc;
			}
		}
		lpSrc += dwSrcPitch;
		lpDst += dwDstPitch;
	}

	hRet = lpDDS->Unlock(NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
}

//// Lock() 스프라이트
//// 기 능 : 16비트 모드 4바이트 전송 스프라이트 출력
//// lpDDS : 소스 표면
//// x, y  : 출력 좌표

void __PutSpriteLockFast16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	DWORD RGBRGB;
	DWORD sRGB1, sRGB2;

	DWORD ix, iy;
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// 소스표면 Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }
    
    //// 목적표면의 RECT
    RECT rcDest = { x, y, x+ddsdSrc.dwWidth, y+ddsdSrc.dwHeight };

	//// 목적표면 Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = g_pDDSBack->Lock(&rcDest, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// 소스표면은 Lock 상태이므로 Unlock
        lpDDS->Unlock(NULL);
        DDERRCHK(ddrval);     
        return;
    }

	//// 표면의 주소
	DWORD* lpDDSSrc = (DWORD*) ddsdSrc.lpSurface;
	DWORD* lpDDSDst = (DWORD*) ddsdDst.lpSurface;
	WORD*  lpDDSDstTemp;

	//// 소스표면의 컬러키 참조
	DWORD dwColorSpaceLowValue = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;	
	
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2; // 4바이트 정렬, /4
	DWORD dwDstPitch = ddsdDst.lPitch >> 2;
	DWORD dwWidth  = ddsdSrc.dwWidth >> 1;   // 2개의 픽셀을 읽으므로 /2
	
	//// BPP
	switch(g_dwRGBBitCount)
	{
	case 16 :
		{
			//// 두개의 픽셀에 대한 컬러키 값을 구한다. 고속 판정을 위한 OR 연산
			DWORD dwColorSpaceLowValueMixed = dwColorSpaceLowValue << 16 | dwColorSpaceLowValue;
	
			if (g_dwPixelFormat == RGB565)
			{
				for (iy=0; iy < ddsdSrc.dwHeight; iy++)
				{
					for (ix=0; ix < dwWidth; ix++)
					{
						//// 두개의 픽셀을 얻어온다
						//memcpy(&RGBRGB, lpDDSSrc+ix, 4);
						RGBRGB = *(lpDDSSrc+ix);
						
						//// 두개의 픽셀이 컬러키이면 루프반복 ( 다음 픽셀 그룹을 얻는다 )
						if (RGBRGB == dwColorSpaceLowValueMixed) continue;	
						
						//// 픽셀을 분리한다
						sRGB1 = RGBRGB & 0x0000FFFF;
						sRGB2 = RGBRGB >> 16;
						
						//// 소스표면의 픽셀 sRGB1,2가 이미지인 경우
						if (sRGB1 != dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue)
						{
							//memcpy(lpDDSDst+ix, &RGBRGB, 4);
							*(lpDDSDst+ix) = RGBRGB;							
						}						
						//// 소스표면의 픽셀 sRGB1이 이미지 픽셀인 경우
						// else if (sRGB1 != dwColorSpaceLowValue && sRGB2 == dwColorSpaceLowValue )
						else if (sRGB1 != dwColorSpaceLowValue)						
						{							
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*lpDDSDstTemp = (WORD)sRGB1;							
						}
						//// 소스표면의 픽셀 sRGB1이 이미지 픽셀인 경우
						// else if (sRGB1 == dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue )
						// else if (sRGB2 != dwColorSpaceLowValue)
						else 
						{
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*(lpDDSDstTemp+1) = (WORD)sRGB2;
						}				
					}					
					lpDDSSrc += dwSrcPitch;
					lpDDSDst += dwDstPitch;
				}
			}	
			else if (g_dwPixelFormat == BGR565)
			{
			}
			else if (g_dwPixelFormat == RGB555)
			{
			}
			break;
		}
	case 24 :
	case 32 : break;
	}	
	
	//// 표면 Unlock
    g_pDDSBack->Unlock(&rcDest);
    lpDDS->Unlock(NULL);
}

//// Blt() 이미지 & 스프라이트 출력 함수의 클립핑 처리 ( 소프트적인 구현 )
//// 기 능 : 스프라이트 출력
//// lpDDS : 소스 표면
//// x, y  : 출력 좌표
//// void __PutImageEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
//// void __PutSpriteEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
void __PutImageEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	//// 표면의 크기 정보를 얻는다 ( 임시코드 )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// 스크린좌표의 RECT를 구한다
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// 스크린 밖으로 나가면 출력하지 않는다
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// 스프라이트가 스크린 밖으로 나간 만큼 출력하지 않는다
	//// 오프 스크린의 사각영역을 감소 시켜 클리핑을 구현한다	

	//// 수평 보정
	if (x < 0)
	{
		// 좌변을 줄인다. rcSrc.left += 넘어간 크기, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;		
	}
	else if (rcDst.right >= HRES)
	{
		// 우변을 줄인다. rcSrc.right -= 넘어간 크기
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// 수직 보정
	if (y < 0)
	{
		// 상변을 줄인다. rcSrc.top += 넘어간 크기, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;		
	}
	else if (rcDst.bottom >= VRES )
	{
		// 하변을 줄인다. rcSrc.bottom -= 넘어간 크기 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	HRESULT ddrval = g_pDDSBack->Blt(&rcDst, lpDDS, &rcSrc, DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void __PutSpriteEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	//// 표면의 크기 정보를 얻는다 ( 임시코드 )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// 스크린좌표의 RECT를 구한다
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// 스크린 밖으로 나가면 출력하지 않는다
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// 스프라이트가 스크린 밖으로 나간 만큼 출력하지 않는다
	//// 오프 스크린의 사각영역을 감소 시켜 클리핑을 구현한다	

	//// 수평 보정
	if (x < 0)
	{
		// 좌변을 줄인다. rcSrc.left += 넘어간 크기, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;
	}
	else if (rcDst.right >= HRES)
	{
		// 우변을 줄인다. rcSrc.right -= 넘어간 크기
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// 수직 보정
	if (y < 0)
	{
		// 상변을 줄인다. rcSrc.top += 넘어간 크기, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;		
	}
	else if (rcDst.bottom >= VRES )
	{
		// 하변을 줄인다. rcSrc.bottom -= 넘어간 크기 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	HRESULT ddrval = g_pDDSBack->Blt(&rcDst, lpDDS, &rcSrc, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//// Lock() 이미지 & 스프라이트 출력 함수의 클립핑 처리 ( 소프트적인 구현 )
//// 기 능 : 16, 32비트 모드 스프라이트 출력
//// lpDDS : 소스 표면
//// x, y  : 출력 좌표
//// void __PutImageLockEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
//// void __PutSpriteLockEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
//// void __PutSpriteLockEx32(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
void __PutSpriteLockFastEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	//// 클립핑 영역구하기
	//// 표면의 정보 얻기
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// 스크린좌표의 RECT를 구한다
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// 스크린 밖으로 나가면 출력하지 않는다
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// 스프라이트가 스크린 밖으로 나간 만큼 출력하지 않는다
	//// 오프 스크린의 사각영역을 감소 시켜 클리핑을 구현한다	

	//// 수평 보정
	if (x < 0)
	{
		// 좌변을 줄인다. rcSrc.left += 넘어간 크기, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;		
	}
	else if (rcDst.right >= HRES)
	{
		// 우변을 줄인다. rcSrc.right -= 넘어간 크기
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// 수직 보정
	if (y < 0)
	{
		// 상변을 줄인다. rcSrc.top += 넘어간 크기, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;
	}
	else if (rcDst.bottom >= VRES )
	{
		// 하변을 줄인다. rcSrc.bottom -= 넘어간 크기 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	////////////////////// 
	DWORD RGBRGB;
	DWORD sRGB1, sRGB2;

	DWORD ix, iy;
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// 소스표면 Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDS->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }
    
    //// 목적표면의 RECT
    //RECT rcDest = { x, y, x+ddsdSrc.dwWidth, y+ddsdSrc.dwHeight };

	//// 목적표면 Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = g_pDDSBack->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// 소스표면은 Lock 상태이므로 Unlock
        lpDDS->Unlock(&rcSrc);
        DDERRCHK(ddrval);
        return;
    }

	//// 표면의 주소
	DWORD* lpDDSSrc = (DWORD*) ddsdSrc.lpSurface;
	DWORD* lpDDSDst = (DWORD*) ddsdDst.lpSurface;
	WORD*  lpDDSDstTemp;

	//// 소스표면의 컬러키 참조
	DWORD dwColorSpaceLowValue = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	
	////
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2; // 4바이트 정렬, /4
	DWORD dwDstPitch = ddsdDst.lPitch >> 2;

	DWORD dwWidthSrc = rcSrc.right-rcSrc.left;
	DWORD dwWidth  = dwWidthSrc >> 1;       // 2개의 픽셀을 읽으므로 /2
	DWORD dwHeight = rcSrc.bottom-rcSrc.top;
	//// 홀수폭인 경우 나머지 플래그 ( 1 픽셀 )
	BOOL bIsRemain = FALSE;
	if (dwWidthSrc % 2) bIsRemain = TRUE;

	//// BPP
	switch(g_dwRGBBitCount)
	{
	case 16 :
		{
			//// 두개의 픽셀에 대한 컬러키 값을 구한다. 고속 판정을 위한 OR 연산
			DWORD dwColorSpaceLowValueMixed = dwColorSpaceLowValue << 16 | dwColorSpaceLowValue;

			if (g_dwPixelFormat == RGB565)
			{
				for (iy=0; iy < dwHeight; iy++)
				{
					for (ix=0; ix < dwWidth; ix++)
					{
						//// 두개의 픽셀을 얻어온다
						//memcpy(&RGBRGB, lpDDSSrc+ix, 4);
						RGBRGB = *(lpDDSSrc+ix);
						
						//// 두개의 픽셀이 컬러키이면 루프반복 ( 다음 픽셀 그룹을 얻는다 )
						if (RGBRGB == dwColorSpaceLowValueMixed) continue;	
						
						//// 픽셀을 분리한다
						sRGB1 = RGBRGB & 0x0000FFFF;
						sRGB2 = RGBRGB >> 16;
						
						//// 소스표면의 픽셀 sRGB1,2가 이미지인 경우
						if (sRGB1 != dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue)
						{
							//memcpy(lpDDSDst+ix, &RGBRGB, 4);
							*(lpDDSDst+ix) = RGBRGB;							
						}						
						//// 소스표면의 픽셀 sRGB1이 이미지 픽셀인 경우
						// else if (sRGB1 != dwColorSpaceLowValue && sRGB2 == dwColorSpaceLowValue )
						else if (sRGB1 != dwColorSpaceLowValue)
						{							
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*lpDDSDstTemp = (WORD)sRGB1;							
						}
						//// 소스표면의 픽셀 sRGB1이 이미지 픽셀인 경우
						// else if (sRGB1 == dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue )
						// else if (sRGB2 != dwColorSpaceLowValue)
						else 
						{
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*(lpDDSDstTemp+1) = (WORD)sRGB2;
						}

						//// 홀수폭, 1 픽셀
						// if (bIsRemain && ix == dwWidth-1)						
						if (ix == dwWidth-1)
						{
							if (bIsRemain)
							{								
								sRGB1 = *(lpDDSSrc+ix+1) & 0x0000FFFF;
								if (sRGB1 != dwColorSpaceLowValue)
								{
									lpDDSDstTemp = (WORD*)(lpDDSDst+ix+1);
									*lpDDSDstTemp = (WORD)sRGB1;
								}
							}
						}
					}
					lpDDSSrc += dwSrcPitch;
					lpDDSDst += dwDstPitch;
				}
			}	
			else if (g_dwPixelFormat == BGR565)
			{
			}
			else if (g_dwPixelFormat == RGB555)
			{
			}
			break;
		}
	case 24 :
	case 32 : break;
	}
	
	//// 표면 Unlock
    g_pDDSBack->Unlock(&rcDst);
    lpDDS->Unlock(&rcSrc);
}

void __PutSpriteLockFastCkEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y, DWORD dwColorKey2)
{
	//// 클립핑 영역구하기
	//// 표면의 정보 얻기
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// 스크린좌표의 RECT를 구한다
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// 스크린 밖으로 나가면 출력하지 않는다
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// 스프라이트가 스크린 밖으로 나간 만큼 출력하지 않는다
	//// 오프 스크린의 사각영역을 감소 시켜 클리핑을 구현한다	

	//// 수평 보정
	if (x < 0)
	{
		// 좌변을 줄인다. rcSrc.left += 넘어간 크기, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;		
	}
	else if (rcDst.right >= HRES)
	{
		// 우변을 줄인다. rcSrc.right -= 넘어간 크기
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// 수직 보정
	if (y < 0)
	{
		// 상변을 줄인다. rcSrc.top += 넘어간 크기, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;
	}
	else if (rcDst.bottom >= VRES )
	{
		// 하변을 줄인다. rcSrc.bottom -= 넘어간 크기 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	////////////////////// 
	DWORD RGBRGB;
	DWORD sRGB1, sRGB2;

	DWORD ix, iy;
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// 소스표면 Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDS->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }
    
    //// 목적표면의 RECT
    //RECT rcDest = { x, y, x+ddsdSrc.dwWidth, y+ddsdSrc.dwHeight };

	//// 목적표면 Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = g_pDDSBack->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// 소스표면은 Lock 상태이므로 Unlock
        lpDDS->Unlock(&rcSrc);
        DDERRCHK(ddrval);
        return;
    }

	//// 표면의 주소
	DWORD* lpDDSSrc = (DWORD*) ddsdSrc.lpSurface;
	DWORD* lpDDSDst = (DWORD*) ddsdDst.lpSurface;
	WORD*  lpDDSDstTemp;

	//TRACE1("%x\n", lpDDSSrc);

	//// 소스표면의 컬러키 참조
	DWORD dwColorSpaceLowValue = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	
	////
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2; // 4바이트 정렬, /4
	DWORD dwDstPitch = ddsdDst.lPitch >> 2;

	DWORD dwWidthSrc = rcSrc.right-rcSrc.left;
	DWORD dwWidth  = dwWidthSrc >> 1;       // 2개의 픽셀을 읽으므로 /2
	DWORD dwHeight = rcSrc.bottom-rcSrc.top;
	//// 홀수폭인 경우 나머지 플래그 ( 1 픽셀 )
	BOOL bIsRemain = FALSE;
	if (dwWidthSrc % 2) bIsRemain = TRUE;

	//// BPP
	switch(g_dwRGBBitCount)
	{
	case 16 :
		{
			//// 두개의 픽셀에 대한 컬러키 값을 구한다. 고속 판정을 위한 OR 연산
			DWORD dwColorSpaceLowValueMixed = dwColorSpaceLowValue << 16 | dwColorSpaceLowValue;
			DWORD dwColorSpaceLowValueMixed2 = dwColorKey2 << 16 | dwColorKey2;

			if (g_dwPixelFormat == RGB565)
			{
				for (iy=0; iy < dwHeight; iy++)
				{
					for (ix=0; ix < dwWidth; ix++)
					{
						//// 두개의 픽셀을 얻어온다
						//memcpy(&RGBRGB, lpDDSSrc+ix, 4);
						RGBRGB = *(lpDDSSrc+ix);
						
						//// 두개의 픽셀이 컬러키이면 루프반복 ( 다음 픽셀 그룹을 얻는다 )
						if (RGBRGB == dwColorSpaceLowValueMixed) continue;
						if (RGBRGB == dwColorSpaceLowValueMixed2) continue;
						
						//// 픽셀을 분리한다
						sRGB1 = RGBRGB & 0x0000FFFF;
						sRGB2 = RGBRGB >> 16;
						
						//// 소스표면의 픽셀 sRGB1,2가 이미지인 경우
						if ((sRGB1 != dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue) &&
							(sRGB1 != dwColorKey2          && sRGB2 != dwColorKey2))
						{
							//memcpy(lpDDSDst+ix, &RGBRGB, 4);
							*(lpDDSDst+ix) = RGBRGB;							
						}
						//// 소스표면의 픽셀 sRGB1이 이미지 픽셀인 경우
						// else if (sRGB1 != dwColorSpaceLowValue && sRGB2 == dwColorSpaceLowValue )
						else if (sRGB1 != dwColorSpaceLowValue && sRGB1 != dwColorKey2)
						{							
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*lpDDSDstTemp = (WORD)sRGB1;							
						}
						//// 소스표면의 픽셀 sRGB1이 이미지 픽셀인 경우
						// else if (sRGB1 == dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue )
						else if (sRGB2 != dwColorSpaceLowValue && sRGB2 != dwColorKey2)
						//else 
						{
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*(lpDDSDstTemp+1) = (WORD)sRGB2;
						}


						//// 홀수폭, 1 픽셀
						// if (bIsRemain && ix == dwWidth-1)						
						if (ix == dwWidth-1)
						{
							if (bIsRemain)
							{								
								sRGB1 = *(lpDDSSrc+ix+1) & 0x0000FFFF;
								if (sRGB1 != dwColorSpaceLowValue && sRGB1 != dwColorKey2)
								{
									lpDDSDstTemp = (WORD*)(lpDDSDst+ix+1);
									*lpDDSDstTemp = (WORD)sRGB1;
								}
							}
						}
					}
					lpDDSSrc += dwSrcPitch;
					lpDDSDst += dwDstPitch;
				}
			}	
			else if (g_dwPixelFormat == BGR565)
			{
			}
			else if (g_dwPixelFormat == RGB555)
			{
			}
			break;
		}
	case 24 :
	case 32 : break;
	}
	
	//// 표면 Unlock
    g_pDDSBack->Unlock(&rcDst);
    lpDDS->Unlock(&rcSrc);	
}

///////////////////////////////////////////////////////////////////////////////
BOOL __Lock(LPDIRECTDRAWSURFACE7 pDDSDest)
{	
    ::ZeroMemory(&g_ddsd, sizeof(DDSURFACEDESC2));
	g_ddsd.dwSize = sizeof(DDSURFACEDESC2);
	HRESULT hRet = pDDSDest->Lock(NULL, &g_ddsd, DDLOCK_WAIT, NULL);	
    if (hRet != DD_OK) { return DDERRCHK(hRet); }
	
	return TRUE;
}

BOOL __Unlock(LPDIRECTDRAWSURFACE7 pDDSDest)
{
	HRESULT hRet = pDDSDest->Unlock(NULL);
	if (hRet != DD_OK) { return DDERRCHK(hRet); }

	return TRUE;
}

//// 좌표 공식 : y * 표면의 가로폭 + x
////             y * 표면의 가로폭 + (x * 픽셀크기)

void __PutPixel8(int x, int y, BYTE bColor)
{
	LPBYTE lpDDS = (LPBYTE) g_ddsd.lpSurface;
	lpDDS[y * g_ddsd.lPitch + x] = bColor;
}

void __PutPixel16(int x, int y, WORD wColor)
{
	LPBYTE lpDDS = (LPBYTE) g_ddsd.lpSurface;
	WORD* lpDst = (WORD*) (lpDDS + y*g_ddsd.lPitch+(x * 2));

	*lpDst = wColor;
}

void __PutPixel32(int x, int y, DWORD dwColor)
{
	LPBYTE lpDDS = (LPBYTE) g_ddsd.lpSurface;
	
	DWORD* lpDst = (DWORD*) (lpDDS + y*g_ddsd.lPitch+(x * 4));	

	*lpDst = dwColor;	
}

WORD __GetPixel16(int x, int y)
{
	LPBYTE lpDDS = (LPBYTE) g_ddsd.lpSurface;
	WORD* pPixel = (WORD*) (lpDDS + y*g_ddsd.lPitch+(x * 2));
	
	return *pPixel;
}

////////////////////////////
//// 32, 64, 32 단계 11111
WORD __GetPixelRGB16(int R, int G, int B)
{
	WORD wPixel=0;

	if (g_dwPixelFormat == RGB565)
	{
		wPixel = (R<<11) | (G<<5) | B;   // 0000 0000 0001 1111		                                 
		return wPixel;					 // 0000 0111 1110 0000   5
	}                                    // 1111 1000 0000 0000  11
	else if (g_dwPixelFormat == RGB555)
	{
		wPixel = (R<<10) | (G<<5) | B;
		return wPixel;
	}
	else // BGR565
	{
		wPixel = (B<<11) | (G<<5) | R;
		return wPixel;
	}
}

///////////////////////////
//// 라인 그리기

//// 수직라인
void __DrawVLine16(int x, int y1, int y2, WORD wColor)
{
	LPBYTE lpDDS = (LPBYTE) g_ddsd.lpSurface;	
	WORD* lpDst = (WORD*) (lpDDS + y1*g_ddsd.lPitch+(x * 2));	
	
	int nPitch = g_ddsd.lPitch>>1; // g_ddsd.lPitch/2
	for (int iy=y1; iy <= y2; iy++)
	{
		*lpDst = wColor;
		lpDst += nPitch;
	}
}

//// 수평라인
void __DrawHLine16(int x1, int y, int x2, WORD wColor)
{
	LPBYTE lpDDS = (LPBYTE) g_ddsd.lpSurface;
	WORD* lpDst = (WORD*) (lpDDS + y*g_ddsd.lPitch+(x1 * 2));

	for (int ix=x1; ix <= x2; ix++)
	{
		*lpDst = wColor;
		lpDst++;
	}
}

//// 상자
void __DrawRect16(int x1, int y1, int x2, int y2, WORD wColor)
{
    ///// 가로
	__DrawHLine16(x1, y1, x2, wColor);
	__DrawHLine16(x1, y2, x2, wColor);

	///// 세로
	__DrawVLine16(x1, y1+1, y2-1, wColor);
	__DrawVLine16(x2, y1+1, y2-1, wColor);
}

///////////////////////////
//// 영역 복사

//// x, y  : 출력좌표
//// rcSrc : 소스 영역

void __CPYDDS(int x, int y, RECT* rcSrc)
{
	LPBYTE lpSrc = (LPBYTE)g_ddsd.lpSurface + rcSrc->top*g_ddsd.lPitch+(rcSrc->left * 2);
	LPBYTE lpDst = (LPBYTE)g_ddsd.lpSurface + y*g_ddsd.lPitch+(x * 2);

	int nWidth = (rcSrc->right-rcSrc->left+1) * 2;
	for (int iy=rcSrc->top; iy < rcSrc->bottom+1; iy++)
	{
		memcpy(lpDst, lpSrc, nWidth);
		lpSrc += g_ddsd.lPitch;
		lpDst += g_ddsd.lPitch;
	}
}

////////////////////////////

DWORD __CheckRGBBit(LPDIRECTDRAWSURFACE7 lpDDS)
{
	DDSURFACEDESC2 ddsd;
	DWORD dwPixelFormat = 0;

	::ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
    ddsd.dwFlags = DDSD_PIXELFORMAT;
    HRESULT hRet = lpDDS->GetSurfaceDesc(&ddsd);
	if (hRet != DD_OK) { DDERRCHK(hRet); return dwPixelFormat; }
		
	g_dwRGBBitCount = ddsd.ddpfPixelFormat.dwRGBBitCount;
	switch(ddsd.ddpfPixelFormat.dwRBitMask)
	{
	case 0xF800   : dwPixelFormat = RGB565; break;    // 11111000 00000000
	case 0x7C00   : dwPixelFormat = RGB555; break;    // 01111100 00000000
	case 0x001F   : dwPixelFormat = BGR565; break;    // 00000000 00011111
	case 0xFF0000 : dwPixelFormat = RGB888; break;    // 11111111 00000000 00000000
	case 0x0000FF : dwPixelFormat = BGR888; break;    // 00000000 00000000 11111111
	}

	return dwPixelFormat;
}

///////////////////////////////////////////////////////////////////////////////

BOOL __DDInit()
{
    ///////////////////////////////////////////////////////////////////////////
    // Create the main DirectDraw object
    ///////////////////////////////////////////////////////////////////////////		
    DDSURFACEDESC2  ddsd;
    DDSCAPS2        ddscaps;
	HRESULT			hRet;

    hRet = DirectDrawCreateEx(NULL, (VOID**)&g_pDD, IID_IDirectDraw7, NULL);
    if (hRet != DD_OK) return DDERRCHK(hRet);
	
    // Get exclusive mode
    hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (hRet != DD_OK) return DDERRCHK(hRet);        

    // Set the video mode 
    hRet = g_pDD->SetDisplayMode(HRES, VRES, BPP, REF, 0);
    if (hRet != DD_OK) return DDERRCHK(hRet);        

    // Create the primary surface with 1 back buffer
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = BBC;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    if (hRet != DD_OK) return DDERRCHK(hRet);        

    // Get a pointer to the back buffer
    ZeroMemory(&ddscaps, sizeof(ddscaps));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
    if (hRet != DD_OK) return DDERRCHK(hRet);

	/////
	TRACE0("FullMode Init.\n");

	gbIsFull = TRUE;

	return TRUE;
}

BOOL __DDInitWin(BOOL bIsVRAM)
{
	DDSURFACEDESC2 ddsd;
	HRESULT        ddrval;

    /////
    HDC hdc = GetDC(hWnd);
    int nBitPixel = GetDeviceCaps(hdc, BITSPIXEL);
    ReleaseDC(hWnd, hdc);
    if (nBitPixel != nBitPixel)
    {
        TRACE0("윈도우모드 설정 실패 - 현재의 컬러모드와 실행모드가 다릅니다\n");
        return FALSE;
    }

	///// 다이렉트 드로우 객체 생성 
  	LPDIRECTDRAW lpDD;
	ddrval = DirectDrawCreate(NULL, &lpDD, NULL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);
	ddrval = lpDD->QueryInterface(IID_IDirectDraw7, (LPVOID*)&g_pDD);
    if (ddrval != DD_OK) return DDERRCHK(ddrval);
	lpDD->Release();
	
	///// 윈도우모드로 설정
	ddrval = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);
 
	///// 1차표면을 생성
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize            = sizeof(ddsd);
	ddsd.dwFlags           = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE;
	
    ddrval = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);    
	if (ddrval != DD_OK) return DDERRCHK(ddrval);

	///// 2차표면
	g_pDDSBack = __CreateSurface(HRES, VRES, bIsVRAM);

	/////
	TRACE0("WindowMode Init.\n");

	gbIsFull = FALSE;

	return TRUE;
}

BOOL __DDCSetClipperWin()
{
	HRESULT ddrval;

	//// 클립퍼객체 생성
	if (!g_pDDClipper) 
	{
        ddrval = DirectDrawCreateClipper(0, &g_pDDClipper, NULL);		
		if (ddrval != DD_OK) return DDERRCHK(ddrval);
	}

	//// 원도우 핸들에 클립퍼 객체를 적용, 첫번째인자는 0
	ddrval = g_pDDClipper->SetHWnd(0, hWnd);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	//// 클립퍼객체를 1차표면에 적용
	ddrval = g_pDDSPrimary->SetClipper(g_pDDClipper);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	return TRUE;
}

BOOL __DDCSetClipper(LPDIRECTDRAWSURFACE7 lpDDS, int x1, int y1, int x2, int y2)
{
	LPRGNDATA pRd;
	HRESULT ddrval;

	if (!g_pDDClipper) 
	{
        ddrval = DirectDrawCreateClipper(0, &g_pDDClipper, NULL);		
		if (ddrval != DD_OK) return DDERRCHK(ddrval);
	}

	///// RGNDATA크기+ 버퍼크기 만큼 할당
	pRd = (LPRGNDATA) malloc(sizeof(RGNDATAHEADER)+sizeof(RECT));		
	
	/////
	pRd->rdh.dwSize = sizeof(pRd->rdh);	
	pRd->rdh.iType = RDH_RECTANGLES;							    // 사각영역임을 의미
	pRd->rdh.nCount = 1;										    // 1개의 영역데이터
	pRd->rdh.nRgnSize = sizeof(RECT);								// 버퍼데이터의 크기
	SetRect(&(pRd->rdh.rcBound), -8000, -8000, 8000, 8000);		    // 바운더리 영역
	SetRect((RECT*)(pRd->Buffer), x1, y1, x2, y2);					// 원하는 클립영역	

	///// 클립리스트 설정
	ddrval = g_pDDClipper->SetClipList(pRd, 0);
	free(pRd);
    if (ddrval != DD_OK) { return DDERRCHK(ddrval); }
	
	ddrval = lpDDS->SetClipper(g_pDDClipper);
    if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	return TRUE;
}

BOOL __DDCSetClipperList(LPDIRECTDRAWSURFACE7 lpDDS, LPRECT rcClipList, int nCount)
{
	LPRGNDATA pRd;
	HRESULT ddrval;

	if (!g_pDDClipper) 
	{
        ddrval = DirectDrawCreateClipper(0, &g_pDDClipper, NULL);		
		if (ddrval != DD_OK) return DDERRCHK(ddrval);
	}

	///// RGNDATA크기+ 버퍼크기 만큼 할당
	pRd = (LPRGNDATA) malloc(sizeof(RGNDATAHEADER)+sizeof(RECT)*nCount);	

	/////
	pRd->rdh.dwSize = sizeof(RGNDATAHEADER);	
	pRd->rdh.iType = RDH_RECTANGLES;							    // 사각영역임을 의미
	pRd->rdh.nCount = nCount;										// 1개의 영역데이터
	pRd->rdh.nRgnSize = sizeof(RECT) * nCount;						// 버퍼데이터의 크기	
	SetRect(&(pRd->rdh.rcBound), -8000, -8000, 8000, 8000);			// 바운더리 영역		
	memcpy(pRd->Buffer, rcClipList, sizeof(RECT)*nCount);			// 원하는 클립영역

	///// 클립리스트 설정
	ddrval = g_pDDClipper->SetClipList(pRd, 0);
	free(pRd);
    if (ddrval != DD_OK) { return DDERRCHK(ddrval); }	
	ddrval = lpDDS->SetClipper(g_pDDClipper);
    if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	return TRUE;
}
LPDIRECTDRAWSURFACE7 __CreateSurface(DWORD dwWidth, DWORD dwHeight, BOOL bIsVram)
{
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;

	///// 오프스크린을 생성
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	if (bIsVram)
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;

	ddsd.dwHeight = dwHeight;
	ddsd.dwWidth  = dwWidth;

    /////
    LPDIRECTDRAWSURFACE7 lpDDS = NULL;
    ddrval = g_pDD->CreateSurface(&ddsd, &lpDDS, NULL);
    if (ddrval == DDERR_OUTOFVIDEOMEMORY)
    {
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        ddrval = g_pDD->CreateSurface(&ddsd, &lpDDS, NULL);
    }
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return NULL; }

    return lpDDS;
}

HRESULT __DDSWBlt(void)
{
    RECT  rcDest;
	POINT pt = {0, 0};	

	/////
	::GetClientRect(hWnd, &rcDest);
	::ClientToScreen(hWnd, &pt);
	::OffsetRect(&rcDest, pt.x, pt.y);

	return g_pDDSPrimary->Blt(&rcDest, g_pDDSBack, NULL, DDBLT_WAIT, NULL);
}
void __ReleaseAllObjects(void)
{
    if (g_pDD != NULL)
    {
		if (g_pDDSSBuffer2 != NULL)
		{
			g_pDDSSBuffer2->Release();
			g_pDDSSBuffer2 = NULL;
		}		
		if (g_pDDSSBuffer != NULL)
		{
			g_pDDSSBuffer->Release();
			g_pDDSSBuffer = NULL;
		}		
		if (g_pDDSVBuffer != NULL)
		{
			g_pDDSVBuffer->Release();
			g_pDDSVBuffer = NULL;
		}
        if (g_pDDPal != NULL)
        {
            g_pDDPal->Release();
            g_pDDPal = NULL;
        }

        if (g_pDDSOne != NULL)
        {
            g_pDDSOne->Release();
            g_pDDSOne = NULL;
        }

        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;
        }
        g_pDD->Release();
        g_pDD = NULL;
    }
}

///////////////////////////////////////////////

void __GetDC(LPDIRECTDRAWSURFACE7 lpDDSDest)
{    
    lpDDSDest->GetDC(&gHdc);
}

void __ReleaseDC(LPDIRECTDRAWSURFACE7 lpDDSDest)
{
    lpDDSDest->ReleaseDC(gHdc);
}

void __CreateFont(LPCTSTR FontName, int nWidth, int nWeight)
{
    gFont = CreateFont(nWidth, 0,
					    0, 0,
					    nWeight,
					    FALSE,
					    FALSE,
					    FALSE,
					    ANSI_CHARSET,
					    OUT_DEFAULT_PRECIS,
					    CLIP_DEFAULT_PRECIS,
					    NONANTIALIASED_QUALITY,
					    VARIABLE_PITCH,
                        FontName);
}

void __DestroyFont()
{
	if (gFont) 
	{
		DeleteObject(gFont);
		gFont = NULL;
	}
}

void __PutText(int x, int y, COLORREF Color, LPCTSTR pString)
{
	SetBkMode(gHdc, TRANSPARENT);
	HFONT* pOldFont = (HFONT*) SelectObject(gHdc, gFont);
	SetTextColor(gHdc, Color);
	TextOut(gHdc, x, y, pString, strlen(pString));
    SelectObject(gHdc, pOldFont);
}

void __PutTextf(int x, int y, COLORREF Color, char *format, ...)
{
	va_list argptr;
	char    buf[512];

	va_start(argptr, format);	
    vsprintf(buf, format, argptr);
	va_end(argptr);

    SetBkMode(gHdc, TRANSPARENT);	
    HFONT* pOldFont = (HFONT*) SelectObject(gHdc, gFont);
	
	///// 그림자
	SetTextColor(gHdc, RGB(0,0,0));
	TextOut(gHdc, x+1, y+1, buf, strlen(buf));
	
	///// 폰트
	SetTextColor(gHdc, Color);
	TextOut(gHdc, x, y, buf, strlen(buf));
    SelectObject(gHdc, pOldFont);
}

/////////////////

void _FillSurface(LPDIRECTDRAWSURFACE7 lpDDSDest, DWORD dwColor)
{
	DDBLTFX ddbltfx;
    
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(DDBLTFX);	
	ddbltfx.dwFillColor = dwColor;
	
	HRESULT ddrval = lpDDSDest->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void _FillRect(LPDIRECTDRAWSURFACE7 lpDDSDest, int x1, int y1, int x2, int y2, DWORD dwColor)
{
	DDBLTFX ddbltfx;
	RECT rcDest = { x1, y1, x2, y2 };
	
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	HRESULT ddrval = lpDDSDest->Blt(&rcDest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
    if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void _FillRectEx(LPDIRECTDRAWSURFACE7 lpDDSDest, LPRECT lpRect, DWORD dwColor)
{
	DDBLTFX ddbltfx;	
	
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	HRESULT ddrval = lpDDSDest->Blt(lpRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

/////////////////////////
//// x, y : 출력될 좌표
//// nWidth, nHeight : 소스 영역의 크기
//// lprcSrc : 소스 영역의 클립핑 사각영역 ( 출력할 영역 )
//// lprcDst : 목적 영역의 클립핑 사각영역 ( 출력될 영역 )
//// 반환되는 RECT는 표면 좌표계
BOOL __GetClipRect(int x, int y, int nWidth, int nHeight, LPRECT lprcSrc, LPRECT lprcDst)
{
    ///// 표면의 끝 좌표를 구한다
	int xEnd = x + nWidth;
	int yEnd = y + nHeight;

	///// 클립핑 영역을 구한다    
	RECT rW = { 0, 0, HRES, VRES };

    ///// 표면이 스크린 밖으로 나가면 출력하지 않는다
	if (x >= rW.right   || y >= rW.bottom) return FALSE;
	if (xEnd <= rW.left || yEnd <= rW.top) return FALSE;

	///// 오프 스크린의 사각영역을 감소 시켜 클리핑영역을 구현한다
    RECT rS = { 0, 0, nWidth, nHeight };

	if (x < rW.left)      { rS.left   -= x; }
	if (y < rW.top)       { rS.top    -= y; }
	if (xEnd > rW.right)  { rS.right  -= (xEnd - rW.right); }	
	if (yEnd > rW.bottom) { rS.bottom -= (yEnd - rW.bottom); }

    ///// 소스의 유효사각여부
	//if (IsRectEmpty(&rS)) return FALSE;

    //CopyRect(lprcSrc, &rS);
	lprcSrc->left   = rS.left;
	lprcSrc->top    = rS.top;
	lprcSrc->right  = rS.right;
	lprcSrc->bottom = rS.bottom;
	
	//SetRect(lprcDst, x+rS.left, y+rS.top, x+rS.right, y+rS.bottom);
    lprcDst->left  = x+rS.left;
    lprcDst->top   = y+rS.top;
    lprcDst->right = x+rS.right;
    lprcDst->bottom= y+rS.bottom;
    
    return TRUE;
}

//// HW 지원이 없으면 실행오류
BOOL __DDSSetColorKeyEx(LPDIRECTDRAWSURFACE7  lpDDSDest, COLORREF rgb1, COLORREF rgb2)
{
    DDCOLORKEY              ddck;

    ddck.dwColorSpaceLowValue = DDColorMatch(lpDDSDest, rgb1);
    ddck.dwColorSpaceHighValue = DDColorMatch(lpDDSDest, rgb2);
	HRESULT ddrval = lpDDSDest->SetColorKey(DDCKEY_SRCBLT | DDCKEY_COLORSPACE, &ddck);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }
	return TRUE;    
}

BOOL __CreateFastVBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

    if (g_pDDSVBuffer) 
	{
		ddrval = g_pDDSVBuffer->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

    g_pDDSVBuffer = __CreateSurface(dwWidth, dwHeight, TRUE);
    if (!g_pDDSVBuffer) return FALSE;

	if (g_dwRGBBitCount >= 16)
	{		
		ddrval = DDSetColorKey(g_pDDSVBuffer, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

    return TRUE;
}

BOOL __CreateFastSBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

    if (g_pDDSSBuffer) 
	{
		ddrval = g_pDDSSBuffer->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

    g_pDDSSBuffer = __CreateSurface(dwWidth, dwHeight, FALSE);
    if (!g_pDDSSBuffer) return FALSE;

	if (g_dwRGBBitCount >= 16)
	{		
		ddrval = DDSetColorKey(g_pDDSSBuffer, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

    return TRUE;
}

BOOL __CreateFastSBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb)
{
	HRESULT ddrval;

    if (g_pDDSSBuffer2) 
	{
		ddrval = g_pDDSSBuffer2->Release();
		if (ddrval != DD_OK) { DDERRCHK(ddrval); return FALSE; }
	}

    g_pDDSSBuffer2 = __CreateSurface(dwWidth, dwHeight, FALSE);
    if (!g_pDDSSBuffer2) return FALSE;

	if (g_dwRGBBitCount >= 16)
	{		
		ddrval = DDSetColorKey(g_pDDSSBuffer2, rgb);
		if (ddrval != DD_OK) DDERRCHK(ddrval);
	}

    return TRUE;
}



//// 이중 컬러키를 이용한 스프라이트 출력
// 구현 개념 
// DDSSrc -> VBUFF ( CK1 )
// VBUFF  -> BACK  ( CK2 )

void __PutSpriteCk(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	ASSERT(lpDDS != NULL);

	//// 표면의 크기 정보를 얻는다 ( 임시코드 )
	DDSURFACEDESC2 ddsdSrc, ddsdVBF;
    ddsdSrc.dwSize = sizeof(ddsdSrc);
	ddsdVBF.dwSize = sizeof(ddsdVBF);
	lpDDS->GetSurfaceDesc(&ddsdSrc);
	g_pDDSVBuffer->GetSurfaceDesc(&ddsdVBF);

	//// 클립핑 영역을 구한다
	// 표면 재전송의 최소화 ( src -> vbuff )
    RECT rSrc, rcDst;
    if (!__GetClipRect(x, y, ddsdSrc.dwWidth, ddsdSrc.dwHeight, &rSrc, &rcDst)) return;

    /////
    if (g_pDDSVBuffer)
    {
		//// DDSVBuffer를 컬러키로 채운다 ( Clean Up )
        _FillRectEx(g_pDDSVBuffer, &rSrc, ddsdVBF.ddckCKSrcBlt.dwColorSpaceLowValue);

		//// 소스표면을 DDSVBuffer로 전송 ( 컬러키1 )
        HRESULT ddrval = g_pDDSVBuffer->Blt(&rSrc, lpDDS, &rSrc, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
        if (ddrval != DD_OK) DDERRCHK(ddrval);

		//// DDSVBuffer를 목적표면( 2차표면 )으로 전송 ( 컬러키2 )
        ddrval = g_pDDSBack->Blt(&rcDst, g_pDDSVBuffer, &rSrc, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
        if (ddrval != DD_OK) DDERRCHK(ddrval);	
    }
    else
    {
		//// 에뮬레이션
    }
}

//// RGB 분해 함수 ( inline function )
void GetRGB565(DWORD RGB, DWORD* pR, DWORD* pG, DWORD* pB)
{	
	*pR = (RGB & 0x0000F800) >> 11;
	*pG = (RGB & 0x000007E0) >> 5;
	*pB = (RGB & 0x0000001F);
}

//// RGB 조합 함수 ( inline function )
DWORD GetPixelRGB565(DWORD R, DWORD G, DWORD B)
{
	return ( (R<<11) | (G<<5) | B );
}

//// 소스 표면을 FI, 목적표면에 출력
//// 소스 표면과 목적표면의 제약을 두지 않는다
//// dwRateStep : 0 부터 100까지 증가하는 값의 크기 ( Step ), 값이 작을수록 부드럽다
//// * 함수 내부에서 0 부터 100까지 루프를 돌린다
void __FxFadeIn(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRateStep)
{	
	/*DWORD RGB1;
	DWORD RGB2;
	DWORD R, G, B;
	DWORD RGBRGB;
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;
	HRESULT ddrval;
	DWORD  ix, iy, dwRate;
	
	if (g_dwRGBBitCount == 16)
	{										
		R = R * dwRate/100; 
		G = G * dwRate/100; 
		B = B * dwRate/100;
	}*/
}

//// 소스 표면을 FO, 목적표면에 출력
//// FI 참조
void __FxFadeOut(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRateStep)
{	
	/*
	DWORD RGB1;
	DWORD RGB2;
	DWORD R, G, B;
	DWORD RGBRGB;
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;
	HRESULT ddrval;
	DWORD  ix, iy, dwRate;
	
	if (g_dwRGBBitCount == 16)
	{
										
		R = R * dwRate/100; 
		G = G * dwRate/100; 
		B = B * dwRate/100; 					
	}
	*/
}

//// 소스표면을 FI, FO, 목적표면에 출력
//// dwRate : 비율을 직접 인자로 넘긴다
//// * 루프는 함수 밖에서 돌린다. FPS 갱신 주기마다 증감시키면 실시간으로 효과가 적용된다
void __FxFadeInOutEx(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate)
{	
	/*
	DWORD RGB1;
	DWORD RGB2;
	DWORD R, G, B;
	DWORD RGBRGB;
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;
	HRESULT ddrval;
	DWORD  ix, iy;
	
	if (g_dwRGBBitCount == 16)
	{
		//// 고속처리를 위해 Look Up Table 참조 ( 미리 계산된 결과, DXW_LUTInit() )
		R = g_dwFADELUT[R][dwRate];
		G = g_dwFADELUT[G][dwRate];
		B = g_dwFADELUT[B][dwRate];
	}
	*/
}

//// 소스표면을 R2, G2, B2의 색으로 변화 시켜 목적표면에 출력
//// * 루프는 함수 내부에서 돌린다, 변형된 값은 지속시켜야 증감이 단계적으로 누적 계산된다. ( 주석참조 )
void __FxFadeTo(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD R2, DWORD G2, DWORD B2)
{	
	/*
	DWORD R, G, B;
	DWORD RGB1;
	DWORD RGB2;	
	DWORD RGBRGB;
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;
	HRESULT ddrval;
	DWORD  ix, iy, i;
	
	if (g_dwRGBBitCount == 16)
	{		
		RGBRGB = *(lpEffectSrc+ix);

		// 픽셀 변형 ( RGB 증감 )

		RGBRGB = (RGB2 << 16) | RGB1;
		
		*(lpEffectSrc+ix) = RGBRGB;
		*(lpEffectDst+ix) = RGBRGB;		
	}
	*/
}

//// 소스표면을 회색 계열로 변형시켜 목적표면으로 출력 ( 단계적 변화 없음 )
void __FxGrayScale(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc)
{	
	/*
	DWORD R, G, B;
	DWORD RGB1,RGB2, Gray;
	DWORD RGBRGB;
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;
	HRESULT ddrval;
	DWORD  ix, iy;
	
	if (g_dwRGBBitCount == 16)
	{
		// Gray = (R + (G/2) + B) / 3;
		// RGB1 = GetPixelRGB565(Gray, Gray*2, Gray);
	}
	*/
}

void __FxColorize(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD R2, DWORD G2, DWORD B2)
{	
	/*
	DWORD R, G, B;
	DWORD RGB1,RGB2;
	DWORD RGBRGB;
	DDSURFACEDESC2 ddsdEffectSrc, ddsdEffectDst;
	HRESULT ddrval;
	DWORD  ix, iy;
	
	if (g_dwRGBBitCount == 16)
	{											
		// R = R * R2 / 32
		// G = G * G2 / 64
		// B = B * B2 / 32				
	}
	*/
}