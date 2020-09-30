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
	//	�ʱ�ȭ
	void Init(HWND _hWnd);
	void Destroy();
	//	Wave���� �ε�
	bool LoadWave(LPSTR _SoundFileName, IDirectSoundBuffer** _pSB);

public:
	//	���
	void Play(IDirectSoundBuffer* _pSB, bool _bLoop = false);
	//	����
	void Stop(IDirectSoundBuffer* _pSB);
	//	����
	void SetVolume(IDirectSoundBuffer* _pSB, LONG _lVolume);

public:
	CHHSound(void);
	~CHHSound(void);
};

#endif
#pragma once
