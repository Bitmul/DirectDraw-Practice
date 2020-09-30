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
	HMMIO WaveFile;		// wave ������ handle

						// Wave������ ��� MMIO Handle�� ��´�.
	WaveFile = mmioOpen(_SoundFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (WaveFile == NULL) return false;

	// �θ� ûũ
	MMCKINFO mmcParent;

	// ������ ���� ûũ�̸��� ����ϰ�, ���� ��ġ�� RIFFûũ���� WAVEûũ�� ã�� ��������.
	mmcParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if ((mmioDescend(WaveFile, &mmcParent, NULL, MMIO_FINDRIFF)) != 0)
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	// �ڽ� ûũ
	MMCKINFO mmcChild;

	// ������ ���� ûũ�̸��� ����ϰ�, ���� ��ġ�� WAVEûũ���� fmt ûũ�� ã�� ��������.
	// ���� : ��� ûũ�� 4���� �����ڵ带 ���� ������ t ������ ���鹮�ڰ� �ִ�.
	mmcChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if ((mmioDescend(WaveFile, &mmcChild, &mmcParent, MMIO_FINDCHUNK) != 0))
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	PCMWAVEFORMAT pcmWaveFormat;

	// fmt ûũ���� wav���� ����( Format )�� �о� ���δ�.
	if (mmioRead(WaveFile, (HPSTR)&pcmWaveFormat, sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	WAVEFORMATEX* pWaveFormat = NULL;
	pWaveFormat = new WAVEFORMATEX;
	memcpy(pWaveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
	pWaveFormat->cbSize = 0;

	// fmt ûũ���� �θ� ûũ�� WAVE ûũ�� �ö󰣴�.
	if (mmioAscend(WaveFile, &mmcChild, 0))
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	// ������ ���� ûũ�̸��� ����ϰ�, ���� ��ġ�� WAVEûũ���� data ûũ�� ã�� ��������.
	mmcChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(WaveFile, &mmcChild, &mmcParent, MMIO_FINDCHUNK) != 0)
	{
		mmioClose(WaveFile, 0);
		return false;
	}

	BYTE* pData = NULL;
	// data ûũ ������ ��ŭ �޸� �Ҵ�
	pData = new BYTE[mmcChild.cksize];

	// data ûũ�� �ִ� ������ wave data�� �о� ���δ�.
	mmioRead(WaveFile, (LPSTR)pData, mmcChild.cksize);

	// wave ������ �ݴ´�.
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

	// ������ Wave �����͸� �����ϱ� ���� ��
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