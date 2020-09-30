#include "HHsound.h"

//: m_pSound( NULL )
CHHSound::CHHSound(void)
{
}

CHHSound::~CHHSound(void)
{
	Destroy();
}

void CHHSound::Init(HWND _hWnd)
{
	DirectSoundCreate(NULL, &m_pSound, NULL);
	m_pSound->SetCooperativeLevel(_hWnd, DSSCL_NORMAL);
}

bool CHHSound::LoadWave(LPSTR _SoundFileName, IDirectSoundBuffer** _pSB)
{
	// RIFF( Resource InterChange File Format )
	HMMIO WaveFile;		// wave 파일의 handle

						// Wave파일을 열어서 MMIO Handle을 얻는다.
	WaveFile = mmioOpen(_SoundFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (WaveFile == NULL) return false;

	// 부모 청크
	MMCKINFO mmcParent;

	// 내려갈 하위 청크이름을 등록하고, 현재 위치인 RIFF청크에서 WAVE청크를 찾아 내려간다.
	mmcParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if ((mmioDescend(WaveFile, &mmcParent, NULL, MMIO_FINDRIFF)) != 0)
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	// 자식 청크
	MMCKINFO mmcChild;

	// 내려갈 하위 청크이름을 등록하고, 현재 위치인 WAVE청크에서 fmt 청크를 찾아 내려간다.
	// 주의 : 모든 청크는 4개의 문자코드를 갖기 때문에 t 다음에 공백문자가 있다.
	mmcChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if ((mmioDescend(WaveFile, &mmcChild, &mmcParent, MMIO_FINDCHUNK) != 0))
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	PCMWAVEFORMAT pcmWaveFormat;

	// fmt 청크에서 wav파일 포맷( Format )을 읽어 들인다.
	if (mmioRead(WaveFile, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	WAVEFORMATEX* pWaveFormat = NULL;
	pWaveFormat = new WAVEFORMATEX;
	memcpy(pWaveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
	pWaveFormat->cbSize = 0;

	// fmt 청크에서 부모 청크인 WAVE 청크로 올라간다.
	if (mmioAscend(WaveFile, &mmcChild, 0))
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	// 내려갈 하위 청크이름을 등록하고, 현재 위치인 WAVE청크에서 data 청크를 찾아 내려간다.
	mmcChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(WaveFile, &mmcChild, &mmcParent, MMIO_FINDCHUNK) != 0)
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	BYTE* pData = NULL;
	// data 청크 사이즈 만큼 메모리 할당
	pData = new BYTE[mmcChild.cksize];

	// data 청크에 있는 순수한 wave data를 읽어 들인다.
	mmioRead(WaveFile, (LPSTR)pData, mmcChild.cksize);

	// wave 파일을 닫는다.
	mmioClose(WaveFile, 0);

	DSBUFFERDESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.dwSize = sizeof(DSBUFFERDESC);
	dsDesc.dwFlags = DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	dsDesc.dwBufferBytes = mmcChild.cksize;
	dsDesc.lpwfxFormat = pWaveFormat;

	m_pSound->CreateSoundBuffer(&dsDesc, _pSB, NULL);

	VOID* pBuff1 = NULL;
	VOID* pBuff2 = NULL;
	DWORD dwLength1;
	DWORD dwLength2;

	// 순수한 Wave 데이터를 복사하기 위해 락
	if ((*_pSB)->Lock(0, dsDesc.dwBufferBytes, &pBuff1, &dwLength1, &pBuff2, &dwLength2, 0L) != DS_OK)
	{
		(*_pSB)->Release();
		(*_pSB) = NULL;
		return false;
	}

	memcpy(pBuff1, pData, dwLength1);
	memcpy(pBuff2, (pData + dwLength1), dwLength2);

	(*_pSB)->Unlock(pBuff1, dwLength1, pBuff2, dwLength2);
	pBuff1 = pBuff2 = NULL;

	delete[] pData;
	delete pWaveFormat;

	return false;
}

void CHHSound::Play(IDirectSoundBuffer* _pSB, bool _bLoop /* = false */)
{
	if (_pSB == NULL) return;

	if (!_pSB->Play(0, 0, (_bLoop) ? 1 : 0)) return;
}

void CHHSound::Stop(IDirectSoundBuffer* _pSB)
{
	if (_pSB == NULL) return;
	_pSB->Stop();

	_pSB->SetCurrentPosition(0L);
}

void CHHSound::SetVolume(IDirectSoundBuffer* _pSB, LONG _lVolume)
{
	_pSB->SetVolume(DSVLOUME_TO_DB(_lVolume));
}

void CHHSound::Destroy()
{
	if (m_pSound != NULL)
	{
		m_pSound->Release();
		m_pSound = NULL;
	}
}