#include "DxMng.h"

#define NAME "DxWFrame"
#define TITLE "DxWFrame"

#define WHITE    RGB(255,255,255)
#define BLACK    RGB(  0,  0,  0)
#define RED      RGB(255,  0,  0)
#define GREEN    RGB(  0,255,  0)
#define BLUE     RGB(  0,  0,255)

///////////////////////////////////////

#ifndef __APP_SET__

typedef struct tagAPP_SET
{
	BOOL bIsPause;	
	BOOL bIsActive;
	BOOL bIsDisplayInfo;
	BOOL bIsRenderBlank;
	int  nBitPixel;
} APP_SET;

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////

class _DXWFRAME_H_ {} ;

BOOL DXW_Init(void);
void DXW_MainLoop(void);
void DXW_DisplayInfo(void);
void DXW_ReleaseWinMode(void);
LRESULT CALLBACK __WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void __WinKeyDown(HWND hwnd, WPARAM wParam);
void __WinDestroy(void);

///////////////////////////////////////////////////////////////

BOOL __DDInit(void);
BOOL __DDInitWin(BOOL bIsVRAM);
HRESULT __DDSWBlt(void);
void __ReleaseAllObjects(void);

LPDIRECTDRAWSURFACE7 __CreateSurface(DWORD dwWidth, DWORD dwHeight, BOOL bIsVram);
BOOL				 __DDCSetClipperWin(void);
BOOL				 __DDCSetClipper(LPDIRECTDRAWSURFACE7 lpDDS, int x1, int y1, int x2, int y2);
BOOL				 __DDCSetClipperList(LPDIRECTDRAWSURFACE7 lpDDS, LPRECT rcClipList, int nCount);

void __CreateFont(LPCTSTR FontName, int nWidth, int nWeight);
void __DestroyFont(void);
void __PutText(int x, int y, COLORREF Color, LPCTSTR pString);
void __PutTextf(int x, int y, COLORREF Color, char *format, ...);
void __GetDC(LPDIRECTDRAWSURFACE7 lpDDSDest);
void __ReleaseDC(LPDIRECTDRAWSURFACE7 lpDDSDest);

/////////////////////////////

inline void _FillSurface(LPDIRECTDRAWSURFACE7 lpDDSDest, DWORD dwColor);
inline void _FillRect(LPDIRECTDRAWSURFACE7 lpDDSDest, int x1, int y1, int x2, int y2, DWORD dwColor);
inline void _FillRectEx(LPDIRECTDRAWSURFACE7 lpDDSDest, LPRECT lpRect, DWORD dwColor);

/////////////////////////////

BOOL __Lock(LPDIRECTDRAWSURFACE7 pDDSDest);
BOOL __Unlock(LPDIRECTDRAWSURFACE7 pDDSDest);

void __PutPixel32(int x, int y, DWORD dwColor);
void __PutPixel16(int x, int y, WORD wColor);
void __PutPixel8(int x, int y, BYTE bColor);

WORD __GetPixel16(int x, int y);

void __DrawVLine16(int x, int y1, int y2, WORD wColor);
void __DrawHLine16(int x1, int y, int x2, WORD wColor);
void __DrawRect16(int x1, int y1, int x2, int y2, WORD wColor);

DWORD __CheckRGBBit(LPDIRECTDRAWSURFACE7 lpdds);
WORD __GetPixelRGB16(int R, int G, int B);
void __CPYDDS(int x, int y, RECT* rcSrc);

LPDIRECTDRAWSURFACE7 __DDLoadBin(LPCSTR strFile, BOOL bIsVRAM);
BOOL __MEM2DDS(LPDIRECTDRAWSURFACE7 lpSurface, LPBYTE lpMem, int nWidth, int nHeight);
void __PutImage(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __PutSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __Putbar(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y,int Mhp, int hp, float HPrate);
void __PutAnimeSprite(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y, int Count, int Number, int cutx, int cuty, int Flip);
void __PutImageLock(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __PutSpriteLock16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __PutSpriteLock32(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);

void __PutImageEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __PutSpriteEx(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);

void __PutSpriteLockFast16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __PutSpriteLockFastEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);

BOOL __DDSSetColorKeyEx(LPDIRECTDRAWSURFACE7  lpDDSDest, COLORREF rgb1, COLORREF rgb2);
BOOL __GetClipRect(int x, int y, int nWidth, int nHeight, LPRECT lprcSrc, LPRECT lprcDst);
BOOL __CreateFastVBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);
BOOL __CreateFastSBuffer(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);
BOOL __CreateFastSBuffer2(DWORD dwWidth, DWORD dwHeight, COLORREF rgb);
void __PutSpriteCk(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y);
void __PutSpriteLockFastCkEx16(LPDIRECTDRAWSURFACE7 lpDDS, int x, int y, DWORD dwColorKey2);

inline void __LockMemcpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc);
inline void __BltCpy(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc);

inline void GetRGB565(DWORD RGB, DWORD* pR, DWORD* pG, DWORD* pB);
inline DWORD GetPixelRGB565(DWORD R, DWORD G, DWORD B);
void DXW_LUTInit(void);
void __FxFadeIn(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRateStep);
void __FxFadeOut(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate);
void __FxFadeInOutEx(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dwRate);
void __FxFadeTo(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD R, DWORD G, DWORD B);
void __FxGrayScale(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc);
void __FxColorize(LPDIRECTDRAWSURFACE7 lpDDSDst, LPDIRECTDRAWSURFACE7 lpDDSSrc, DWORD dR, DWORD dG, DWORD dB);