#include <dsound.h>

#ifndef _CHHSOUND_H
#define _CHHSOUND_H

#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME)
#endif


#define DSVLOUME_TO_DB(VOLUME) ((DWORD)(-30*(100-VOLUME)))

class CHHSound
{
private:
	IDirectSound * m_pSound;

public:
	//	√ ±‚»≠
	void Init(HWND _hWnd);
	void Destroy();
	//	Wave∆ƒ¿œ ∑ŒµÂ
	bool LoadWave(LPSTR _SoundFileName, IDirectSoundBuffer** _pSB);

public:
	//	¿Áª˝
	void Play(IDirectSoundBuffer* _pSB, bool _bLoop = false);
	//	∏ÿ√„
	void Stop(IDirectSoundBuffer* _pSB);
	//	∫º∑˝
	void SetVolume(IDirectSoundBuffer* _pSB, LONG _lVolume);

public:
	CHHSound(void);
	~CHHSound(void);
};

#endif
#pragma once
