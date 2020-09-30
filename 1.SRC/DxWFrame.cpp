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

//// ��Ʈ �� ������ ����
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

POINT		g_Pos,g_BPos; //�÷��̾��� ��ġ,������ ��ġ || ����� Point �ڷ����� x�� y��ǥ�� �������ִ� ����ü�Դϴ�.
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

//g_bJump = ������ �ߴ°�?
//g_bAccel = �������ΰ�?
//g_bJaccel = ���� �������ΰ�?
//g_bRight | g_bLeft = ������ �Ǵ� �������� �̵����ΰ�?
//g_bJRight | g_bJLeft = ������ �Ǵ� �������� �������ΰ�?
//g_bSetSpeed = ���ӵ� �Ѱ�ġ�� ���� �ƴ°�?
//g_bLookR | g_bLookL = ������ �Ǵ� ������ �����ִ°�?
//g_BLR | g_BLL = ������ ���� �Ǵ� �������� �����ִ°�?
//g_bAR | g_bAL = ������ �Ǵ� �������� �������ΰ�?
//g_bAtt = �������ΰ�?
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
int Gravity = 13; //�߷� ����
int Hspeed = 0; //���ӵ� ����
int P1MHP = 3000; //������ 1 ü��
int P1HP = P1MHP;
int P2MHP = 5000;
int P2HP = P2MHP;
int P3MHP = 10000;
int P3HP = P3MHP;
int cx, cy = 0; //�߻�ü ��ǥ
int o = 0;
int P3randpat = 0;
int P3randpat2 = 0;
BOOL isSafe = FALSE;
BOOL g_bCast = FALSE;
BOOL g_bCRight = FALSE;
BOOL g_bCLeft = FALSE;
BOOL g_bCFire = FALSE;
BOOL g_bHit = FALSE;
BOOL Mirror = FALSE; //���� ����
BOOL BossAtt = FALSE;
BOOL BAR, BAL = FALSE;
BOOL BARR, BALL = FALSE;
BOOL Muffle = FALSE;
BOOL P5Mus = FALSE;
BOOL CastHit = FALSE;
BOOL Skilluse = FALSE;
BOOL BigHit = FALSE; //�ʻ�� ��Ÿ
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
int KnockHspeed = 0; //�˹鰡�ӵ�
int Bframe = 0; //���� �ִ� ������
int BAframe = 0; //���� ���ݾִ� ������
int Aframe = 0; //���� �ִ� ������
int Cframe = 0; //���� �ִ� ������
int CAframe = 0; //������ �ִ� ������
int CAAframe = 0; //�߻�ü �ִ� ������
int BKframe = 0; //���� �˹���� �ִ� ������
int DRframe = 0;//���� �ִ� ������
int GAframe = 0;//����� �ִ� ������
int SPframe = 0;//���� �ִ� ������
int PHframe = 0;//�÷��̾� �ǰݾִ� ������
int BHframe[3] = { 0 };//���� �ǰݾִ� ������
int PSframe = 0; //�÷��̾� �ʻ�� �ִ� ������
int SpikeX[10] = { 0 };
int SpikeY = 0;
int MaxTime = 500; //��Ÿ�� �ð�
int Skillcheat = 0; //�ʻ�� ����
float Timerrate = 1;
BOOL Pimmune = FALSE;
RECT HIT,BOSS,rcDest,BAHIT[5],PHIT,Cast,Knock,Spike[10];
BOOL p1die = FALSE; //������ 1 ������ �׾��°�?
BOOL p2die = FALSE;
BOOL p3die = FALSE;
BOOL Drag = FALSE;
BOOL Knockback = FALSE; // �˹���
BOOL KnockHIT = FALSE; //�˹� �Ѵ� ����
BOOL p3musicstart = FALSE;
BOOL P4Mus = FALSE;
BOOL g_bGatt = FALSE; //����� �������ΰ�?
BOOL g_bSpike = FALSE; //�ٴ� ���� �������ΰ�?
BOOL PHplay = FALSE; //�÷��̾�/���� �ǰ� �ִϰ� ������ΰ�?
BOOL BHplay[3] = { FALSE,};
BOOL BHMPlay = FALSE; //���� �ǰݾִ���?
int g_nPhaze = 0; //����� 1 ������� ����
int Timer[32] = { 0,}; //0��° Ÿ�̸� - ĳ���� �̵� 2��° - ���� �̵� 3��° - ���� �ִϸ��̼� 4��° - �÷��̾� ���� 5��° - ���� �ǰݹ��� 6��° - ���� �ִ�
//7��° - �Ź̴ٸ� ���� ��Ÿ�� 8��° - �÷��̾� ������ 9��° - ������ ��Ÿ�� 10��° - ĳ���� �ִϸ��̼� 11��° - ĳ���� ���� �ִ� 12��° - ���� ĳ���� �ð�
//13��°- 2������ ĳ���� 14��° - ���� ���� �ð� 15��° - ���� ���� ��Ÿ�� 16��° - ���� �˹� �ִ� 17��° - ���� �˹� ��Ÿ��
//18��° - ���� ���� �ִ� 19��° - ���� ���� ��Ÿ�� 20��° - ���� �ٴ���� ���� ��Ÿ�� 21��° - ���� �ٴ���� ���� �ִ� 22��° - �ٴڰ��� ���� ��Ÿ�� 23��° - �ٴڰ��� ���� �ִ�
//24��° - ���� �ǰ� �ִ� 25��° - �÷��̾� �ǰ� �ִ� 26��° - ���� �ǰ� �ִ� 2 27��° ���� �ǰ� �ִ� 3 28��° �ʻ�� ��Ÿ�� 29��° �ʻ�� �ִ�
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

		//���� �ʱ�ȭ
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
		
		///// �Է���ġ �ʱ�ȭ
	///// �Է���ġ �ʱ�ȭ


	/////
    BOOL rval = DXW_Init();
    if (!rval) return FALSE;

    return TRUE;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    MSG		msg;
    
	//// â��� ����

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
	//// ����� ��带 ���� ��Ʈ�ȼ� ����

	////
	DXW_ReleaseWinMode();
	
	////
	__DestroyFont();

	//// DX ������Ʈ ����
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
	//// Fade Look Up Table ����
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
    ///// DX ��ü ���� �� �ʱ�ȭ
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
	
	///// �ȼ����� üũ
	g_dwPixelFormat = ::__CheckRGBBit(g_pDDSPrimary);

	///// ������ �ҷ�����
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
	g_pDDSCast = ::DDLoadBitmap(g_pDD, "image/Cast3.bmp", 3840,192, TRUE); //�ִϸ��̼� 20��.
	::DDSetColorKey(g_pDDSCast, RGB(0, 255, 0));
	g_pDDSCast2 = ::DDLoadBitmap(g_pDD, "image/Cast4.bmp", 3840, 192, TRUE); //�ִϸ��̼� 20��.
	::DDSetColorKey(g_pDDSCast2, RGB(255, 0, 255));
	g_pDDSCastAtt = ::DDLoadBitmap(g_pDD, "image/CastAttack.bmp", 3440, 525, TRUE); //�ִϸ��̼� 10��.
	::DDSetColorKey(g_pDDSCastAtt, RGB(0,255,0));
	g_pDDSBossAtt = ::DDLoadBitmap(g_pDD, "image/BossAttackMotionL.bmp", 7116, 321, TRUE); //�ִϸ��̼� 12��.
	::DDSetColorKey(g_pDDSBossAtt, RGB(0, 255, 0));
	g_pDDSBossAttR = ::DDLoadBitmap(g_pDD, "image/BossAttackMotionR.bmp", 7116, 321, TRUE); //�ִϸ��̼� 12��.
	::DDSetColorKey(g_pDDSBossAttR, RGB(0, 255, 0));
	g_pDDSDragAttR = ::DDLoadBitmap(g_pDD, "image/BossAttack2R.bmp", 12240, 315, TRUE); //�ִϸ��̼� 17��.
	::DDSetColorKey(g_pDDSDragAttR, RGB(0, 255, 0));
	g_pDDSDragAtt = ::DDLoadBitmap(g_pDD, "image/BossAttack2L.bmp", 12240, 315, TRUE); //�ִϸ��̼� 17��.
	::DDSetColorKey(g_pDDSDragAtt, RGB(0, 255, 0));
	g_pDDSPlayerskill = ::DDLoadBitmap(g_pDD, "image/Worldreaver.bmp", 20480, 768, TRUE); //�ִϸ��̼� 20��.
	::DDSetColorKey(g_pDDSPlayerskill, RGB(0, 255, 0));
	g_pDDSBoss3Att = ::DDLoadBitmap(g_pDD, "image/Boss3AttackL.bmp", 16100, 384, TRUE); //�ִϸ��̼� 24��.
	::DDSetColorKey(g_pDDSBoss3Att, RGB(0, 255, 0));
	g_pDDSBoss3AttR = ::DDLoadBitmap(g_pDD, "image/Boss3AttackR.bmp", 16100, 384, TRUE); //�ִϸ��̼� 24��.
	::DDSetColorKey(g_pDDSBoss3AttR, RGB(0, 255, 0));
	g_pDDSBoss3Att3 = ::DDLoadBitmap(g_pDD, "image/Boss3Attack33L.bmp", 35247, 798, TRUE); //�ִϸ��̼� 31��.
	::DDSetColorKey(g_pDDSBoss3Att3, RGB(0, 255, 0));
	g_pDDSBoss3AttR3 = ::DDLoadBitmap(g_pDD, "image/Boss3Attack33R.bmp", 35247, 798, TRUE); //�ִϸ��̼� 31��.
	::DDSetColorKey(g_pDDSBoss3AttR3, RGB(0, 255, 0));
	g_pDDSGroundAtt= ::DDLoadBitmap(g_pDD, "image/GroundAttack.bmp", 6330, 629, TRUE); //�ִϸ��̼� 15��.
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
	if (!g_pDDSOne) return MSGERROR("������ũ����������");
	g_pDDSTITLE = ::DDLoadBitmap(g_pDD, "image/Title.bmp", HRES, VRES, TRUE);
	g_pDDSBG = ::DDLoadBitmap(g_pDD, "image/MI1.bmp", HRES, VRES, TRUE);
	g_pDDSBG2 = ::DDLoadBitmap(g_pDD, "image/MI2.bmp", HRES, VRES, TRUE);
	g_pDDSBG3 = ::DDLoadBitmap(g_pDD, "image/MI3.bmp", HRES, VRES, TRUE);
	g_pDDSGameover = ::DDLoadBitmap(g_pDD, "image/GameOver.bmp", HRES, VRES, TRUE);
	g_pDDSEnd = ::DDLoadBitmap(g_pDD, "image/End.bmp", HRES, VRES, TRUE);
	//// VRAM Cache ����, �÷�Ű2
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
	///// ��Ÿ
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

	//// ����ó��
	WORD wPixel = __GetPixelRGB16(0, 0, 0);
	int nX, bnX; //�÷��̾�� ������ X��ǥ
	int nY, bnY; //�÷��̾�� ������ Y��ǥ
	int SnX, SnY; //��ų�� ǥ���� X,Y��ǥ.
	int i = 0;
	if (g_nPhaze >= 1 && Knockback == FALSE && PlayerHP > 1 && g_nPhaze < 4)
	{
		if (::GetKeyState(VK_LEFT) & 0x80 && g_bJump == 0 && g_bAtt == FALSE && g_bRush == FALSE) //���� �̵� ����
		{
			Hspeed -= 2;
			g_bAccel = TRUE;
			g_bLeft = TRUE;
			g_bRight = FALSE;
			g_bLookL = TRUE;
			g_bLookR = FALSE;
		}
		else if (g_Pos.x > 0 && g_bJump == 1) //���� �� ����
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
			else if (g_bJRight == 0 && ::GetKeyState(VK_LEFT) & 0x80) //���� �� ����
			{
				if (Hspeed > -2)
				{
					Hspeed -= 1;
				}
			}
		}
		if (g_bJLeft == 1 && ::GetKeyState(VK_RIGHT) & 0x80) //���� �� �ݴ� Ű�� �������� ����.
		{
			if (Hspeed > -3)
			{
				Hspeed += 1;
				g_bSetSpeed = TRUE; //���� �Ѱ�ġ ����
			}
			else
			{
				Hspeed = -3;
			}
		}
		//////////////////
		if (::GetKeyState(VK_RIGHT) & 0x80 && g_bJump == 0 && g_bAtt == FALSE && g_bRush == FALSE) //������ �̵� ����
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
		//�ƹ� Ű�� �Է����� �ʾ��� ��� ����
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
		//���� ����
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
		//���ӵ� �ִ�ġ ����
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
		//ȭ�� ������ �̵��ϴ°��� ������.
		//�߷� ���� ��������
	}
	if (g_bJump)
	{
		if (Gravity > -20)
		{
			Gravity -= 1;
			g_Pos.y -= Gravity;
			if (g_Pos.y > 590 && g_nPhaze == 1) //�ٴ�
			{
				g_Pos.y = 590;
				Gravity = 13;
				g_bSetSpeed = FALSE;
				g_bJump = FALSE;
				g_bJaccel = FALSE;
				g_bJLeft = FALSE;
				g_bJRight = FALSE;
			}
			if (g_Pos.y > 500 && g_nPhaze == 2) //�ٴ�
			{
				g_Pos.y = 500;
				Gravity = 13;
				g_bSetSpeed = FALSE;
				g_bJump = FALSE;
				g_bJaccel = FALSE;
				g_bJLeft = FALSE;
				g_bJRight = FALSE;
			}
			if (g_Pos.y > 510 && g_nPhaze == 3) //�ٴ�
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
			//���
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
			//�̵� �� ���� ����
			///////////////////////
			///////////////////////
			///////////////////////
			///////////////////////
			//				g_BPos.x += 1;

				if (g_BPos.x + 60 < g_Pos.x && g_bCast == FALSE) //���� ������ ��ȯ
				{
					g_bBLR = TRUE;
					g_bBLL = FALSE;
				}
				else if (g_BPos.x > g_Pos.x && g_bCast == FALSE) //���� ���� ��ȯ
				{
					g_bBLR = FALSE;
					g_bBLL = TRUE;
				}
				if (g_BPos.x + 300 < g_Pos.x && g_bCast == FALSE) //���� ������ �̵�
				{
					if (BossAtt == FALSE)
					{
						g_BPos.x += 1;
					}
					g_bBLR = TRUE;
					g_bBLL = FALSE;
				}
				else if (g_BPos.x - 200 > g_Pos.x && g_bCast == FALSE) //���� ���� �̵�
				{
					if (BossAtt == FALSE)
					{
						g_BPos.x -= 1;
					}
					g_bBLR = FALSE;
					g_bBLL = TRUE;
				}
				if (::GetKeyState(0x58) & 0x80 && g_bAtt == FALSE && g_bRush == FALSE) //���� ����
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
			//�̵� �� ���� ���� ��
			//// ǥ�� Ŭ����	
			//_FillSurface(g_pDDSBack, 0x00000000);
			nX = g_Pos.x;
			nY = g_Pos.y;
			bnX = g_BPos.x;
			bnY = g_BPos.y;
			SnX = nX;
			SnY = nY - 65;
			//�÷��̾� �ǰݷ�Ʈ ����
			if (g_bLookL)
			{
				PHIT = { nX + 11,nY + 10,nX + 45,nY + 80 };
			}
			else if (g_bLookR)
			{
				PHIT = { nX + 25,nY + 10,nX + 58,nY + 80 };
			}
			//ĳ���� �ִϸ��̼�
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
			//�߻�� ���� �ִϸ��̼�
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
			//// ���� ���ĵ�
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
			//ĳ���� ���� �ִϸ��̼�
			if (g_bRush == FALSE)
			{
				//���ĵ�
				if (g_bLookL == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
				}
				else if (g_bLookR == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
				}
				//����
				if (g_bLookL == 1 && g_bJump == 1 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 3, 86, 81, 0);
				}
				else if (g_bLookR == 1 && g_bJump == 1 && g_bAtt == FALSE)
				{
					::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 5, 86, 81, 0);
				}
				//����
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
			//��Ʈ Ȯ�ο�
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
			///// ��Ÿ���� 
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
						 //���
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
						 //�̵� �� ���� ����
						 ///////////////////////
						 ///////////////////////
						 ///////////////////////
						 ///////////////////////
						 //				g_BPos.x += 1;
						 if (g_BPos.x + 60 < g_Pos.x) //���� ������ ��ȯ
						 {
							 g_bBLR = TRUE;
							 g_bBLL = FALSE;
						 }
						 else if (g_BPos.x > g_Pos.x) //���� ���� ��ȯ
						 {
							 g_bBLR = FALSE;
							 g_bBLL = TRUE;
						 }
						 if (g_BPos.x + 300 < g_Pos.x) //���� ������ �̵�
						 {
							 if (BossAtt == FALSE)
							 {
								 g_BPos.x += 1;
							 }
							 g_bBLR = TRUE;
							 g_bBLL = FALSE;
						 }
						 else if (g_BPos.x - 200 > g_Pos.x) //���� ���� �̵�
						 {
							 if (BossAtt == FALSE)
							 {
								 g_BPos.x -= 1;
							 }
							 g_bBLR = FALSE;
							 g_bBLL = TRUE;
						 }
						 if (::GetKeyState(0x58) & 0x80 && g_bAtt == FALSE && g_bRush == FALSE) //���� ����
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
						 //�̵� �� ���� ���� ��
						 //// ǥ�� Ŭ����	
						 //_FillSurface(g_pDDSBack, 0x00000000);
						 nX = g_Pos.x;
						 nY = g_Pos.y;
						 bnX = g_BPos.x;
						 bnY = g_BPos.y;
						 SnX = nX;
						 SnY = nY - 65;
						 //ĳ���� �ִϸ��̼�
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
						 //�÷��̾� �ǰݷ�Ʈ ����
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
						 //// ���� ���ĵ�

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
						 //ĳ���� ���� �ִϸ��̼�
						 if (g_bRush == FALSE)
						 {
							 //���ĵ�
							 if (g_bLookL == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
							 }
							 else if (g_bLookR == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
							 }
							 //����
							 if (g_bLookL == 1 && g_bJump == 1 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 3, 86, 81, 0);
							 }
							 else if (g_bLookR == 1 && g_bJump == 1 && g_bAtt == FALSE)
							 {
								 ::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 5, 86, 81, 0);
							 }
							 //����
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
						 //��Ʈ Ȯ�ο�
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
						 ///// ��Ÿ���� 
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
												//���
													::__PutImage(g_pDDSBG3, Bx, 0);
												if (Timer[11] > 0)
												{
													Timer[11] -= 1;
												}
												if (Timer[12] > 0)
												{
													Timer[12] -= 1;
												}
												//�̵� �� ���� ����
												///////////////////////
												///////////////////////
												///////////////////////
												///////////////////////
												//				g_BPos.x += 1;

												if (g_BPos.x + 60 < g_Pos.x) //���� ������ ��ȯ
												{
													g_bBLR = TRUE;
													g_bBLL = FALSE;
												}
												else if (g_BPos.x > g_Pos.x - 60) //���� ���� ��ȯ
												{
													g_bBLR = FALSE;
													g_bBLL = TRUE;
												}
												if (g_BPos.x + 300 < g_Pos.x) //���� ������ �̵�
												{
													if (BossAtt == FALSE && g_bGatt == FALSE)
													{
														g_BPos.x += 1;
													}
													g_bBLR = TRUE;
													g_bBLL = FALSE;
												}
												else if (g_BPos.x - 200 > g_Pos.x) //���� ���� �̵�
												{
													if (BossAtt == FALSE && g_bGatt == FALSE)
													{
														g_BPos.x -= 1;
													}
													g_bBLR = FALSE;
													g_bBLL = TRUE;
												}
												if (::GetKeyState(0x58) & 0x80 && g_bAtt == FALSE && g_bRush == FALSE) //���� ����
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
												//�̵� �� ���� ���� ��
												//// ǥ�� Ŭ����	
												//_FillSurface(g_pDDSBack, 0x00000000);
												nX = g_Pos.x;
												nY = g_Pos.y;
												bnX = g_BPos.x;
												bnY = g_BPos.y;
												SnX = nX;
												SnY = nY - 65;
												//�÷��̾� �ǰݷ�Ʈ ����
												if (g_bLookL)
												{
													PHIT = { nX + 11,nY + 10,nX + 45,nY + 80 };
												}
												else if (g_bLookR)
												{
													PHIT = { nX + 25,nY + 10,nX + 58,nY + 80 };
												}
												//// ���� ���ĵ�
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
												//ĳ���� ���� �ִϸ��̼�
												if (g_bRush == FALSE)
												{
													//���ĵ�
													if (g_bLookL == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 0, 86, 81, 0);
													}
													else if (g_bLookR == 1 && g_bJump == 0 && g_bLeft == 0 && g_bRight == 0 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 8, 86, 81, 0);
													}
													//����
													if (g_bLookL == 1 && g_bJump == 1 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOne, nX, nY, 9, 3, 86, 81, 0);
													}
													else if (g_bLookR == 1 && g_bJump == 1 && g_bAtt == FALSE)
													{
														::__PutAnimeSprite(g_pDDSOneR, nX, nY, 9, 5, 86, 81, 0);
													}
													//����
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
												//��Ʈ Ȯ�ο�
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
												///// ��Ÿ���� 
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
	///// �ø��� �Ǵ� ���۸�
#ifdef _DEBUG
	hRet = __DDSWBlt();
#elif _NOVSYNC
	hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_NOVSYNC);
#else
	hRet = g_pDDSPrimary->Flip(NULL, DDFLIP_WAIT);
#endif

	///// ǥ�麹��	
	if (hRet == DDERR_SURFACELOST) g_pDD->RestoreAllSurfaces();
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }

	///// ������ ����
	gFpsMng.FrameTuningEx();

}

void DXW_DisplayInfo()
{
	///// �����Ӽ� ���� �� ��Ÿ ����
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

    //// �ҽ�ǥ���� RECT
    ddsdSrc.dwSize = sizeof(ddsdSrc);  	
	lpDDSSrc->GetSurfaceDesc(&ddsdSrc);
    RECT rcSrc = { 0, 0, ddsdSrc.dwWidth, 1 };
	DWORD dwWidth = ddsdSrc.dwWidth;	
	
	//// �ҽ�ǥ�� Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDSSrc->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }

    //// ����ǥ���� RECT
    ddsdDst.dwSize = sizeof(ddsdDst);    	
	lpDDSDst->GetSurfaceDesc(&ddsdDst);
    RECT rcDst = { 0, 0, ddsdDst.dwWidth, 1 };        

	//// ����ǥ�� Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = lpDDSDst->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// �ҽ�ǥ���� Lock �����̹Ƿ� Unlock
        lpDDSSrc->Unlock(&rcSrc);
        DDERRCHK(ddrval);     
        return;
    }

	//// ǥ���� �ּ�
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
//// �̹�������Ÿ ǥ�� ����
//// 

LPDIRECTDRAWSURFACE7 __DDLoadBin(LPCSTR strFile, BOOL bIsVRAM)
{
	SIZE szBin;
	int nSize;

	//// ���� �б�
	//// 8 ����Ʈ : ����, ����ũ��
	//// 4 ����Ʈ : �̹��� ��ü ũ��
	FILE* file = fopen(strFile, "rb");
	fread(&szBin, sizeof(SIZE), 1, file);
	fread(&nSize, sizeof(int), 1, file);	
	LPBYTE lpImg = (LPBYTE) malloc(nSize);
	fread(lpImg, nSize, 1, file);
	fclose(file);

	///// ������ũ�� ���� ( VRAM )
	LPDIRECTDRAWSURFACE7 lpDDS = NULL;
	lpDDS = __CreateSurface(szBin.cx, szBin.cy, bIsVRAM);
	if (!lpDDS) { MSGERROR("_DDLoadBin"); return NULL; }

	///// ������ ������ũ���� �޸� ����Ÿ ����
	BOOL bRval = __MEM2DDS(lpDDS, lpImg, szBin.cx, szBin.cy);
	if (!bRval) { MSGERROR("_DDLoadBin"); return NULL; }

	free(lpImg);

	return lpDDS;
}

//// �޸� -> ǥ��
//// 

BOOL __MEM2DDS(LPDIRECTDRAWSURFACE7 lpDstDDS, LPBYTE lpSrcMem, int nWidth, int nHeight)
{
	int iy;
	DDSURFACEDESC2 ddsd;
	HRESULT hRet;
		
	int nWidthStep=0;

	//// ��Ʈī��Ʈ�� ���� ���� �������� ���Ѵ�
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

	//// �޸� ���� ( ���� )
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

//// Blt() �̹���
////
void __PutImage(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{	
	//// ǥ���� ũ�� ������ ��´� ( �ӽ��ڵ� )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	/////
	RECT rcDest = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };	
	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, NULL, DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//// Blt() ��������Ʈ 
////
void __PutSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{	
	//// ǥ���� ũ�� ������ ��´� ( �ӽ��ڵ� )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	/////
	RECT rcDest = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };	
	HRESULT ddrval = g_pDDSBack->Blt(&rcDest, lpDDS, NULL, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//�ִϸ��̼� ��� �Լ�
void __PutAnimeSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y, int Count, int Number, int cutx, int cuty, int Flip)
{
	//// ǥ���� ũ�� ������ ��´� ( �ӽ��ڵ� )
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
	//�¿����
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
//// Lock() �̹���
//// �� �� : ����ǥ��(2��)�� Lock�� �� ���¿��� �Լ� ����
//// �� �� : 16, 32��Ʈ ��� �̹��� ���
//// lpDDS : �ҽ� ǥ��
//// x, y  : ��� ��ǥ 

void __PutImageLock(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddsdSrc;

	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);    

	hRet = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }

	///// BPP�� ���� ���� ���� ���Ѵ�
	DWORD dwMul = 1;
	switch(g_dwRGBBitCount)
	{
	case 16 : dwMul = 2; break;
	case 32 : dwMul = 4; break;
	}

	//// �̹��� ǥ�� �ּ�
	LPBYTE lpSrc = (LPBYTE) ddsdSrc.lpSurface;
	
	//// 2�� ǥ�� �ּ� ( ���� )	
	LPBYTE lpDst = (LPBYTE) g_ddsd.lpSurface + y*g_ddsd.lPitch+(x * dwMul);

	int nWidth  = ddsdSrc.dwWidth * dwMul;
	int nHeight = ddsdSrc.dwHeight;

	//// memcpy�� ���� �޸� ����
	for (int iy=0; iy < nHeight; iy++)
	{
		memcpy(lpDst, lpSrc, nWidth);
		lpSrc += ddsdSrc.lPitch;
		lpDst += g_ddsd.lPitch;
	}

	hRet = lpDDS->Unlock(NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
}

//// Lock() ��������Ʈ
//// �� �� : ����ǥ��(2��)�� Lock�� �� ���¿��� �Լ� ����
//// �� �� : 16��Ʈ ��� ��������Ʈ ���
//// lpDDS : �ҽ� ǥ��
//// x, y  : ��� ��ǥ 

void __PutSpriteLock16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddsdSrc;

	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);    

	hRet = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
	
	//// �̹��� ǥ�� �ּ�
	WORD* lpSrc = (WORD*) ddsdSrc.lpSurface;
	
	//// 2�� ǥ�� �ּ� ( ���� ) , ( *2 )
	WORD* lpDst = (WORD*) ((LPBYTE)g_ddsd.lpSurface + y*g_ddsd.lPitch+(x << 1));

	int nWidth  = ddsdSrc.dwWidth;
	int nHeight = ddsdSrc.dwHeight;
	
	//// �÷�Ű
	DWORD dwSrcColorKey = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	WORD wSrc=0;

	//// WORD ������ �ּҰ� ���� �ǹǷ� RShift ( /2)
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 1;
	DWORD dwDstPitch = g_ddsd.lPitch >> 1;

	//// ������������ �ִ��� ������ �ּ�ȭ�Ѵ�	
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

//// Lock() ��������Ʈ
//// �� �� : ����ǥ��(2��)�� Lock�� �� ���¿��� �Լ� ����
//// �� �� : 32��Ʈ ��� ��������Ʈ ���
//// lpDDS : �ҽ� ǥ��
//// x, y  : ��� ��ǥ 

void __PutSpriteLock32(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddsdSrc;

	memset(&ddsdSrc, 0, sizeof(ddsdSrc));
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	ddsdSrc.dwFlags = DDSD_CKSRCBLT;

	hRet = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
	if (hRet != DD_OK) { DDERRCHK(hRet); return; }
	
	//// �̹��� ǥ�� �ּ�
	DWORD* lpSrc = (DWORD*) ddsdSrc.lpSurface;
	
	//// 2�� ǥ�� �ּ� ( ���� ) , ( *4 )
	DWORD* lpDst = (DWORD*) ((LPBYTE)g_ddsd.lpSurface + y*g_ddsd.lPitch+(x << 2));

	int nWidth  = ddsdSrc.dwWidth;
	int nHeight = ddsdSrc.dwHeight;
	
	//// �÷�Ű
	DWORD dwSrcColorKey = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	DWORD dwSrc=0;

	//// DWORD ������ �ּҰ� ���� �ǹǷ� RShift ( /4)
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2;
	DWORD dwDstPitch = g_ddsd.lPitch >> 2;

	//// ������������ �ִ��� ������ �ּ�ȭ�Ѵ�	
	for (int iy=0; iy < nHeight; iy++)
	{
		for (int ix=0; ix < nWidth; ix++)
		{
			//// ���� �� �Ǵ� �ֻ��� 8��Ʈ�� �����Ѵ�, ARGB
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

//// Lock() ��������Ʈ
//// �� �� : 16��Ʈ ��� 4����Ʈ ���� ��������Ʈ ���
//// lpDDS : �ҽ� ǥ��
//// x, y  : ��� ��ǥ

void __PutSpriteLockFast16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	DWORD RGBRGB;
	DWORD sRGB1, sRGB2;

	DWORD ix, iy;
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// �ҽ�ǥ�� Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDS->Lock(NULL, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }
    
    //// ����ǥ���� RECT
    RECT rcDest = { x, y, x+ddsdSrc.dwWidth, y+ddsdSrc.dwHeight };

	//// ����ǥ�� Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = g_pDDSBack->Lock(&rcDest, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// �ҽ�ǥ���� Lock �����̹Ƿ� Unlock
        lpDDS->Unlock(NULL);
        DDERRCHK(ddrval);     
        return;
    }

	//// ǥ���� �ּ�
	DWORD* lpDDSSrc = (DWORD*) ddsdSrc.lpSurface;
	DWORD* lpDDSDst = (DWORD*) ddsdDst.lpSurface;
	WORD*  lpDDSDstTemp;

	//// �ҽ�ǥ���� �÷�Ű ����
	DWORD dwColorSpaceLowValue = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;	
	
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2; // 4����Ʈ ����, /4
	DWORD dwDstPitch = ddsdDst.lPitch >> 2;
	DWORD dwWidth  = ddsdSrc.dwWidth >> 1;   // 2���� �ȼ��� �����Ƿ� /2
	
	//// BPP
	switch(g_dwRGBBitCount)
	{
	case 16 :
		{
			//// �ΰ��� �ȼ��� ���� �÷�Ű ���� ���Ѵ�. ��� ������ ���� OR ����
			DWORD dwColorSpaceLowValueMixed = dwColorSpaceLowValue << 16 | dwColorSpaceLowValue;
	
			if (g_dwPixelFormat == RGB565)
			{
				for (iy=0; iy < ddsdSrc.dwHeight; iy++)
				{
					for (ix=0; ix < dwWidth; ix++)
					{
						//// �ΰ��� �ȼ��� ���´�
						//memcpy(&RGBRGB, lpDDSSrc+ix, 4);
						RGBRGB = *(lpDDSSrc+ix);
						
						//// �ΰ��� �ȼ��� �÷�Ű�̸� �����ݺ� ( ���� �ȼ� �׷��� ��´� )
						if (RGBRGB == dwColorSpaceLowValueMixed) continue;	
						
						//// �ȼ��� �и��Ѵ�
						sRGB1 = RGBRGB & 0x0000FFFF;
						sRGB2 = RGBRGB >> 16;
						
						//// �ҽ�ǥ���� �ȼ� sRGB1,2�� �̹����� ���
						if (sRGB1 != dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue)
						{
							//memcpy(lpDDSDst+ix, &RGBRGB, 4);
							*(lpDDSDst+ix) = RGBRGB;							
						}						
						//// �ҽ�ǥ���� �ȼ� sRGB1�� �̹��� �ȼ��� ���
						// else if (sRGB1 != dwColorSpaceLowValue && sRGB2 == dwColorSpaceLowValue )
						else if (sRGB1 != dwColorSpaceLowValue)						
						{							
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*lpDDSDstTemp = (WORD)sRGB1;							
						}
						//// �ҽ�ǥ���� �ȼ� sRGB1�� �̹��� �ȼ��� ���
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
	
	//// ǥ�� Unlock
    g_pDDSBack->Unlock(&rcDest);
    lpDDS->Unlock(NULL);
}

//// Blt() �̹��� & ��������Ʈ ��� �Լ��� Ŭ���� ó�� ( ����Ʈ���� ���� )
//// �� �� : ��������Ʈ ���
//// lpDDS : �ҽ� ǥ��
//// x, y  : ��� ��ǥ
//// void __PutImageEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
//// void __PutSpriteEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
void __PutImageEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	//// ǥ���� ũ�� ������ ��´� ( �ӽ��ڵ� )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// ��ũ����ǥ�� RECT�� ���Ѵ�
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// ��ũ�� ������ ������ ������� �ʴ´�
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// ��������Ʈ�� ��ũ�� ������ ���� ��ŭ ������� �ʴ´�
	//// ���� ��ũ���� �簢������ ���� ���� Ŭ������ �����Ѵ�	

	//// ���� ����
	if (x < 0)
	{
		// �º��� ���δ�. rcSrc.left += �Ѿ ũ��, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;		
	}
	else if (rcDst.right >= HRES)
	{
		// �캯�� ���δ�. rcSrc.right -= �Ѿ ũ��
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// ���� ����
	if (y < 0)
	{
		// ���� ���δ�. rcSrc.top += �Ѿ ũ��, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;		
	}
	else if (rcDst.bottom >= VRES )
	{
		// �Ϻ��� ���δ�. rcSrc.bottom -= �Ѿ ũ�� 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	HRESULT ddrval = g_pDDSBack->Blt(&rcDst, lpDDS, &rcSrc, DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

void __PutSpriteEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	//// ǥ���� ũ�� ������ ��´� ( �ӽ��ڵ� )
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// ��ũ����ǥ�� RECT�� ���Ѵ�
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// ��ũ�� ������ ������ ������� �ʴ´�
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// ��������Ʈ�� ��ũ�� ������ ���� ��ŭ ������� �ʴ´�
	//// ���� ��ũ���� �簢������ ���� ���� Ŭ������ �����Ѵ�	

	//// ���� ����
	if (x < 0)
	{
		// �º��� ���δ�. rcSrc.left += �Ѿ ũ��, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;
	}
	else if (rcDst.right >= HRES)
	{
		// �캯�� ���δ�. rcSrc.right -= �Ѿ ũ��
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// ���� ����
	if (y < 0)
	{
		// ���� ���δ�. rcSrc.top += �Ѿ ũ��, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;		
	}
	else if (rcDst.bottom >= VRES )
	{
		// �Ϻ��� ���δ�. rcSrc.bottom -= �Ѿ ũ�� 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	HRESULT ddrval = g_pDDSBack->Blt(&rcDst, lpDDS, &rcSrc, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) DDERRCHK(ddrval);
}

//// Lock() �̹��� & ��������Ʈ ��� �Լ��� Ŭ���� ó�� ( ����Ʈ���� ���� )
//// �� �� : 16, 32��Ʈ ��� ��������Ʈ ���
//// lpDDS : �ҽ� ǥ��
//// x, y  : ��� ��ǥ
//// void __PutImageLockEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
//// void __PutSpriteLockEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
//// void __PutSpriteLockEx32(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
void __PutSpriteLockFastEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	//// Ŭ���� �������ϱ�
	//// ǥ���� ���� ���
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// ��ũ����ǥ�� RECT�� ���Ѵ�
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// ��ũ�� ������ ������ ������� �ʴ´�
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// ��������Ʈ�� ��ũ�� ������ ���� ��ŭ ������� �ʴ´�
	//// ���� ��ũ���� �簢������ ���� ���� Ŭ������ �����Ѵ�	

	//// ���� ����
	if (x < 0)
	{
		// �º��� ���δ�. rcSrc.left += �Ѿ ũ��, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;		
	}
	else if (rcDst.right >= HRES)
	{
		// �캯�� ���δ�. rcSrc.right -= �Ѿ ũ��
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// ���� ����
	if (y < 0)
	{
		// ���� ���δ�. rcSrc.top += �Ѿ ũ��, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;
	}
	else if (rcDst.bottom >= VRES )
	{
		// �Ϻ��� ���δ�. rcSrc.bottom -= �Ѿ ũ�� 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	////////////////////// 
	DWORD RGBRGB;
	DWORD sRGB1, sRGB2;

	DWORD ix, iy;
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// �ҽ�ǥ�� Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDS->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }
    
    //// ����ǥ���� RECT
    //RECT rcDest = { x, y, x+ddsdSrc.dwWidth, y+ddsdSrc.dwHeight };

	//// ����ǥ�� Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = g_pDDSBack->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// �ҽ�ǥ���� Lock �����̹Ƿ� Unlock
        lpDDS->Unlock(&rcSrc);
        DDERRCHK(ddrval);
        return;
    }

	//// ǥ���� �ּ�
	DWORD* lpDDSSrc = (DWORD*) ddsdSrc.lpSurface;
	DWORD* lpDDSDst = (DWORD*) ddsdDst.lpSurface;
	WORD*  lpDDSDstTemp;

	//// �ҽ�ǥ���� �÷�Ű ����
	DWORD dwColorSpaceLowValue = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	
	////
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2; // 4����Ʈ ����, /4
	DWORD dwDstPitch = ddsdDst.lPitch >> 2;

	DWORD dwWidthSrc = rcSrc.right-rcSrc.left;
	DWORD dwWidth  = dwWidthSrc >> 1;       // 2���� �ȼ��� �����Ƿ� /2
	DWORD dwHeight = rcSrc.bottom-rcSrc.top;
	//// Ȧ������ ��� ������ �÷��� ( 1 �ȼ� )
	BOOL bIsRemain = FALSE;
	if (dwWidthSrc % 2) bIsRemain = TRUE;

	//// BPP
	switch(g_dwRGBBitCount)
	{
	case 16 :
		{
			//// �ΰ��� �ȼ��� ���� �÷�Ű ���� ���Ѵ�. ��� ������ ���� OR ����
			DWORD dwColorSpaceLowValueMixed = dwColorSpaceLowValue << 16 | dwColorSpaceLowValue;

			if (g_dwPixelFormat == RGB565)
			{
				for (iy=0; iy < dwHeight; iy++)
				{
					for (ix=0; ix < dwWidth; ix++)
					{
						//// �ΰ��� �ȼ��� ���´�
						//memcpy(&RGBRGB, lpDDSSrc+ix, 4);
						RGBRGB = *(lpDDSSrc+ix);
						
						//// �ΰ��� �ȼ��� �÷�Ű�̸� �����ݺ� ( ���� �ȼ� �׷��� ��´� )
						if (RGBRGB == dwColorSpaceLowValueMixed) continue;	
						
						//// �ȼ��� �и��Ѵ�
						sRGB1 = RGBRGB & 0x0000FFFF;
						sRGB2 = RGBRGB >> 16;
						
						//// �ҽ�ǥ���� �ȼ� sRGB1,2�� �̹����� ���
						if (sRGB1 != dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue)
						{
							//memcpy(lpDDSDst+ix, &RGBRGB, 4);
							*(lpDDSDst+ix) = RGBRGB;							
						}						
						//// �ҽ�ǥ���� �ȼ� sRGB1�� �̹��� �ȼ��� ���
						// else if (sRGB1 != dwColorSpaceLowValue && sRGB2 == dwColorSpaceLowValue )
						else if (sRGB1 != dwColorSpaceLowValue)
						{							
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*lpDDSDstTemp = (WORD)sRGB1;							
						}
						//// �ҽ�ǥ���� �ȼ� sRGB1�� �̹��� �ȼ��� ���
						// else if (sRGB1 == dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue )
						// else if (sRGB2 != dwColorSpaceLowValue)
						else 
						{
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*(lpDDSDstTemp+1) = (WORD)sRGB2;
						}

						//// Ȧ����, 1 �ȼ�
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
	
	//// ǥ�� Unlock
    g_pDDSBack->Unlock(&rcDst);
    lpDDS->Unlock(&rcSrc);
}

void __PutSpriteLockFastCkEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y, DWORD dwColorKey2)
{
	//// Ŭ���� �������ϱ�
	//// ǥ���� ���� ���
	DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    //ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpDDS->GetSurfaceDesc(&ddsd);

	//// ��ũ����ǥ�� RECT�� ���Ѵ�
	RECT rcDst = { x, y, x+ddsd.dwWidth, y+ddsd.dwHeight };
	RECT rcSrc = { 0, 0, ddsd.dwWidth, ddsd.dwHeight };

	//// ��ũ�� ������ ������ ������� �ʴ´�
	if (x >= HRES || y >= VRES) return;
	if (rcDst.right <= 0 || rcDst.bottom <= 0) return;
	
	//// ��������Ʈ�� ��ũ�� ������ ���� ��ŭ ������� �ʴ´�
	//// ���� ��ũ���� �簢������ ���� ���� Ŭ������ �����Ѵ�	

	//// ���� ����
	if (x < 0)
	{
		// �º��� ���δ�. rcSrc.left += �Ѿ ũ��, abs(x)
		rcSrc.left -= x;
		rcDst.left=x=0;		
	}
	else if (rcDst.right >= HRES)
	{
		// �캯�� ���δ�. rcSrc.right -= �Ѿ ũ��
		int nOverSize = (rcDst.right - HRES);		
		rcSrc.right -= nOverSize;
		rcDst.right -= nOverSize;
	}
	//// ���� ����
	if (y < 0)
	{
		// ���� ���δ�. rcSrc.top += �Ѿ ũ��, abs(y)
		rcSrc.top -= y;
		rcDst.top=y=0;
	}
	else if (rcDst.bottom >= VRES )
	{
		// �Ϻ��� ���δ�. rcSrc.bottom -= �Ѿ ũ�� 
		int nOverSize = (rcDst.bottom - VRES);
		rcSrc.bottom -= nOverSize;
		rcDst.bottom -= nOverSize;
	}

	////////////////////// 
	DWORD RGBRGB;
	DWORD sRGB1, sRGB2;

	DWORD ix, iy;
    DDSURFACEDESC2 ddsdSrc, ddsdDst;

	//// �ҽ�ǥ�� Lock
	ZeroMemory(&ddsdSrc, sizeof(ddsdSrc));	
	ddsdSrc.dwSize = sizeof(ddsdSrc);
	HRESULT ddrval = lpDDS->Lock(&rcSrc, &ddsdSrc, DDLOCK_READONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK) { DDERRCHK(ddrval); return; }
    
    //// ����ǥ���� RECT
    //RECT rcDest = { x, y, x+ddsdSrc.dwWidth, y+ddsdSrc.dwHeight };

	//// ����ǥ�� Lock
	ZeroMemory(&ddsdDst, sizeof(ddsdDst));	
	ddsdDst.dwSize = sizeof(ddsdDst);
	ddrval = g_pDDSBack->Lock(&rcDst, &ddsdDst, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);
    if (ddrval != DD_OK)
    {
    	//// �ҽ�ǥ���� Lock �����̹Ƿ� Unlock
        lpDDS->Unlock(&rcSrc);
        DDERRCHK(ddrval);
        return;
    }

	//// ǥ���� �ּ�
	DWORD* lpDDSSrc = (DWORD*) ddsdSrc.lpSurface;
	DWORD* lpDDSDst = (DWORD*) ddsdDst.lpSurface;
	WORD*  lpDDSDstTemp;

	//TRACE1("%x\n", lpDDSSrc);

	//// �ҽ�ǥ���� �÷�Ű ����
	DWORD dwColorSpaceLowValue = ddsdSrc.ddckCKSrcBlt.dwColorSpaceLowValue;
	
	////
	DWORD dwSrcPitch = ddsdSrc.lPitch >> 2; // 4����Ʈ ����, /4
	DWORD dwDstPitch = ddsdDst.lPitch >> 2;

	DWORD dwWidthSrc = rcSrc.right-rcSrc.left;
	DWORD dwWidth  = dwWidthSrc >> 1;       // 2���� �ȼ��� �����Ƿ� /2
	DWORD dwHeight = rcSrc.bottom-rcSrc.top;
	//// Ȧ������ ��� ������ �÷��� ( 1 �ȼ� )
	BOOL bIsRemain = FALSE;
	if (dwWidthSrc % 2) bIsRemain = TRUE;

	//// BPP
	switch(g_dwRGBBitCount)
	{
	case 16 :
		{
			//// �ΰ��� �ȼ��� ���� �÷�Ű ���� ���Ѵ�. ��� ������ ���� OR ����
			DWORD dwColorSpaceLowValueMixed = dwColorSpaceLowValue << 16 | dwColorSpaceLowValue;
			DWORD dwColorSpaceLowValueMixed2 = dwColorKey2 << 16 | dwColorKey2;

			if (g_dwPixelFormat == RGB565)
			{
				for (iy=0; iy < dwHeight; iy++)
				{
					for (ix=0; ix < dwWidth; ix++)
					{
						//// �ΰ��� �ȼ��� ���´�
						//memcpy(&RGBRGB, lpDDSSrc+ix, 4);
						RGBRGB = *(lpDDSSrc+ix);
						
						//// �ΰ��� �ȼ��� �÷�Ű�̸� �����ݺ� ( ���� �ȼ� �׷��� ��´� )
						if (RGBRGB == dwColorSpaceLowValueMixed) continue;
						if (RGBRGB == dwColorSpaceLowValueMixed2) continue;
						
						//// �ȼ��� �и��Ѵ�
						sRGB1 = RGBRGB & 0x0000FFFF;
						sRGB2 = RGBRGB >> 16;
						
						//// �ҽ�ǥ���� �ȼ� sRGB1,2�� �̹����� ���
						if ((sRGB1 != dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue) &&
							(sRGB1 != dwColorKey2          && sRGB2 != dwColorKey2))
						{
							//memcpy(lpDDSDst+ix, &RGBRGB, 4);
							*(lpDDSDst+ix) = RGBRGB;							
						}
						//// �ҽ�ǥ���� �ȼ� sRGB1�� �̹��� �ȼ��� ���
						// else if (sRGB1 != dwColorSpaceLowValue && sRGB2 == dwColorSpaceLowValue )
						else if (sRGB1 != dwColorSpaceLowValue && sRGB1 != dwColorKey2)
						{							
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*lpDDSDstTemp = (WORD)sRGB1;							
						}
						//// �ҽ�ǥ���� �ȼ� sRGB1�� �̹��� �ȼ��� ���
						// else if (sRGB1 == dwColorSpaceLowValue && sRGB2 != dwColorSpaceLowValue )
						else if (sRGB2 != dwColorSpaceLowValue && sRGB2 != dwColorKey2)
						//else 
						{
							lpDDSDstTemp = (WORD*)(lpDDSDst+ix);
							*(lpDDSDstTemp+1) = (WORD)sRGB2;
						}


						//// Ȧ����, 1 �ȼ�
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
	
	//// ǥ�� Unlock
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

//// ��ǥ ���� : y * ǥ���� ������ + x
////             y * ǥ���� ������ + (x * �ȼ�ũ��)

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
//// 32, 64, 32 �ܰ� 11111
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
//// ���� �׸���

//// ��������
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

//// �������
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

//// ����
void __DrawRect16(int x1, int y1, int x2, int y2, WORD wColor)
{
    ///// ����
	__DrawHLine16(x1, y1, x2, wColor);
	__DrawHLine16(x1, y2, x2, wColor);

	///// ����
	__DrawVLine16(x1, y1+1, y2-1, wColor);
	__DrawVLine16(x2, y1+1, y2-1, wColor);
}

///////////////////////////
//// ���� ����

//// x, y  : �����ǥ
//// rcSrc : �ҽ� ����

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
        TRACE0("�������� ���� ���� - ������ �÷����� �����尡 �ٸ��ϴ�\n");
        return FALSE;
    }

	///// ���̷�Ʈ ��ο� ��ü ���� 
  	LPDIRECTDRAW lpDD;
	ddrval = DirectDrawCreate(NULL, &lpDD, NULL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);
	ddrval = lpDD->QueryInterface(IID_IDirectDraw7, (LPVOID*)&g_pDD);
    if (ddrval != DD_OK) return DDERRCHK(ddrval);
	lpDD->Release();
	
	///// ��������� ����
	ddrval = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	if (ddrval != DD_OK) return DDERRCHK(ddrval);
 
	///// 1��ǥ���� ����
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize            = sizeof(ddsd);
	ddsd.dwFlags           = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE;
	
    ddrval = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);    
	if (ddrval != DD_OK) return DDERRCHK(ddrval);

	///// 2��ǥ��
	g_pDDSBack = __CreateSurface(HRES, VRES, bIsVRAM);

	/////
	TRACE0("WindowMode Init.\n");

	gbIsFull = FALSE;

	return TRUE;
}

BOOL __DDCSetClipperWin()
{
	HRESULT ddrval;

	//// Ŭ���۰�ü ����
	if (!g_pDDClipper) 
	{
        ddrval = DirectDrawCreateClipper(0, &g_pDDClipper, NULL);		
		if (ddrval != DD_OK) return DDERRCHK(ddrval);
	}

	//// ������ �ڵ鿡 Ŭ���� ��ü�� ����, ù��°���ڴ� 0
	ddrval = g_pDDClipper->SetHWnd(0, hWnd);
	if (ddrval != DD_OK) { return DDERRCHK(ddrval); }

	//// Ŭ���۰�ü�� 1��ǥ�鿡 ����
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

	///// RGNDATAũ��+ ����ũ�� ��ŭ �Ҵ�
	pRd = (LPRGNDATA) malloc(sizeof(RGNDATAHEADER)+sizeof(RECT));		
	
	/////
	pRd->rdh.dwSize = sizeof(pRd->rdh);	
	pRd->rdh.iType = RDH_RECTANGLES;							    // �簢�������� �ǹ�
	pRd->rdh.nCount = 1;										    // 1���� ����������
	pRd->rdh.nRgnSize = sizeof(RECT);								// ���۵������� ũ��
	SetRect(&(pRd->rdh.rcBound), -8000, -8000, 8000, 8000);		    // �ٿ���� ����
	SetRect((RECT*)(pRd->Buffer), x1, y1, x2, y2);					// ���ϴ� Ŭ������	

	///// Ŭ������Ʈ ����
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

	///// RGNDATAũ��+ ����ũ�� ��ŭ �Ҵ�
	pRd = (LPRGNDATA) malloc(sizeof(RGNDATAHEADER)+sizeof(RECT)*nCount);	

	/////
	pRd->rdh.dwSize = sizeof(RGNDATAHEADER);	
	pRd->rdh.iType = RDH_RECTANGLES;							    // �簢�������� �ǹ�
	pRd->rdh.nCount = nCount;										// 1���� ����������
	pRd->rdh.nRgnSize = sizeof(RECT) * nCount;						// ���۵������� ũ��	
	SetRect(&(pRd->rdh.rcBound), -8000, -8000, 8000, 8000);			// �ٿ���� ����		
	memcpy(pRd->Buffer, rcClipList, sizeof(RECT)*nCount);			// ���ϴ� Ŭ������

	///// Ŭ������Ʈ ����
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

	///// ������ũ���� ����
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
	
	///// �׸���
	SetTextColor(gHdc, RGB(0,0,0));
	TextOut(gHdc, x+1, y+1, buf, strlen(buf));
	
	///// ��Ʈ
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
//// x, y : ��µ� ��ǥ
//// nWidth, nHeight : �ҽ� ������ ũ��
//// lprcSrc : �ҽ� ������ Ŭ���� �簢���� ( ����� ���� )
//// lprcDst : ���� ������ Ŭ���� �簢���� ( ��µ� ���� )
//// ��ȯ�Ǵ� RECT�� ǥ�� ��ǥ��
BOOL __GetClipRect(int x, int y, int nWidth, int nHeight, LPRECT lprcSrc, LPRECT lprcDst)
{
    ///// ǥ���� �� ��ǥ�� ���Ѵ�
	int xEnd = x + nWidth;
	int yEnd = y + nHeight;

	///// Ŭ���� ������ ���Ѵ�    
	RECT rW = { 0, 0, HRES, VRES };

    ///// ǥ���� ��ũ�� ������ ������ ������� �ʴ´�
	if (x >= rW.right   || y >= rW.bottom) return FALSE;
	if (xEnd <= rW.left || yEnd <= rW.top) return FALSE;

	///// ���� ��ũ���� �簢������ ���� ���� Ŭ���ο����� �����Ѵ�
    RECT rS = { 0, 0, nWidth, nHeight };

	if (x < rW.left)      { rS.left   -= x; }
	if (y < rW.top)       { rS.top    -= y; }
	if (xEnd > rW.right)  { rS.right  -= (xEnd - rW.right); }	
	if (yEnd > rW.bottom) { rS.bottom -= (yEnd - rW.bottom); }

    ///// �ҽ��� ��ȿ�簢����
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

//// HW ������ ������ �������
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



//// ���� �÷�Ű�� �̿��� ��������Ʈ ���
// ���� ���� 
// DDSSrc -> VBUFF ( CK1 )
// VBUFF  -> BACK  ( CK2 )

void __PutSpriteCk(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y)
{
	ASSERT(lpDDS != NULL);

	//// ǥ���� ũ�� ������ ��´� ( �ӽ��ڵ� )
	DDSURFACEDESC2 ddsdSrc, ddsdVBF;
    ddsdSrc.dwSize = sizeof(ddsdSrc);
	ddsdVBF.dwSize = sizeof(ddsdVBF);
	lpDDS->GetSurfaceDesc(&ddsdSrc);
	g_pDDSVBuffer->GetSurfaceDesc(&ddsdVBF);

	//// Ŭ���� ������ ���Ѵ�
	// ǥ�� �������� �ּ�ȭ ( src -> vbuff )
    RECT rSrc, rcDst;
    if (!__GetClipRect(x, y, ddsdSrc.dwWidth, ddsdSrc.dwHeight, &rSrc, &rcDst)) return;

    /////
    if (g_pDDSVBuffer)
    {
		//// DDSVBuffer�� �÷�Ű�� ä��� ( Clean Up )
        _FillRectEx(g_pDDSVBuffer, &rSrc, ddsdVBF.ddckCKSrcBlt.dwColorSpaceLowValue);

		//// �ҽ�ǥ���� DDSVBuffer�� ���� ( �÷�Ű1 )
        HRESULT ddrval = g_pDDSVBuffer->Blt(&rSrc, lpDDS, &rSrc, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
        if (ddrval != DD_OK) DDERRCHK(ddrval);

		//// DDSVBuffer�� ����ǥ��( 2��ǥ�� )���� ���� ( �÷�Ű2 )
        ddrval = g_pDDSBack->Blt(&rcDst, g_pDDSVBuffer, &rSrc, DDBLT_KEYSRC|DDBLT_WAIT, NULL);
        if (ddrval != DD_OK) DDERRCHK(ddrval);	
    }
    else
    {
		//// ���ķ��̼�
    }
}

//// RGB ���� �Լ� ( inline function )
void GetRGB565(DWORD RGB, DWORD* pR, DWORD* pG, DWORD* pB)
{	
	*pR = (RGB & 0x0000F800) >> 11;
	*pG = (RGB & 0x000007E0) >> 5;
	*pB = (RGB & 0x0000001F);
}

//// RGB ���� �Լ� ( inline function )
DWORD GetPixelRGB565(DWORD R, DWORD G, DWORD B)
{
	return ( (R<<11) | (G<<5) | B );
}

//// �ҽ� ǥ���� FI, ����ǥ�鿡 ���
//// �ҽ� ǥ��� ����ǥ���� ������ ���� �ʴ´�
//// dwRateStep : 0 ���� 100���� �����ϴ� ���� ũ�� ( Step ), ���� �������� �ε巴��
//// * �Լ� ���ο��� 0 ���� 100���� ������ ������
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

//// �ҽ� ǥ���� FO, ����ǥ�鿡 ���
//// FI ����
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

//// �ҽ�ǥ���� FI, FO, ����ǥ�鿡 ���
//// dwRate : ������ ���� ���ڷ� �ѱ��
//// * ������ �Լ� �ۿ��� ������. FPS ���� �ֱ⸶�� ������Ű�� �ǽð����� ȿ���� ����ȴ�
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
		//// ���ó���� ���� Look Up Table ���� ( �̸� ���� ���, DXW_LUTInit() )
		R = g_dwFADELUT[R][dwRate];
		G = g_dwFADELUT[G][dwRate];
		B = g_dwFADELUT[B][dwRate];
	}
	*/
}

//// �ҽ�ǥ���� R2, G2, B2�� ������ ��ȭ ���� ����ǥ�鿡 ���
//// * ������ �Լ� ���ο��� ������, ������ ���� ���ӽ��Ѿ� ������ �ܰ������� ���� ���ȴ�. ( �ּ����� )
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

		// �ȼ� ���� ( RGB ���� )

		RGBRGB = (RGB2 << 16) | RGB1;
		
		*(lpEffectSrc+ix) = RGBRGB;
		*(lpEffectDst+ix) = RGBRGB;		
	}
	*/
}

//// �ҽ�ǥ���� ȸ�� �迭�� �������� ����ǥ������ ��� ( �ܰ��� ��ȭ ���� )
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