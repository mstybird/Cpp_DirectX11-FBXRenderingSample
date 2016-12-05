#include"MSAudio.hxx"

bool LoadSound(const TCHAR *lpFileName)
{
	MMCKINFO mmckinfo; PCMWAVEFORMAT pcmwf; MMRESULT mmret;

	memset(&g_mmioinfo, 0x00, sizeof(g_mmioinfo));
	g_hmmio = mmioOpen(const_cast<TCHAR *>(lpFileName), &g_mmioinfo, MMIO_READ);
	if (g_hmmio == NULL) return false;

	memset(&g_riffchunkinfo, 0x00, sizeof(g_riffchunkinfo));
	g_riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmret = mmioDescend(g_hmmio, &g_riffchunkinfo, NULL, MMIO_FINDRIFF);
	if (mmret != MMSYSERR_NOERROR) return false;

	memset(&mmckinfo, 0x00, sizeof(mmckinfo));
	mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmret = mmioDescend(g_hmmio, &mmckinfo, &g_riffchunkinfo, MMIO_FINDCHUNK);
	if (mmret != MMSYSERR_NOERROR) return false;

	if (mmckinfo.cksize >= sizeof(WAVEFORMATEX)) { mmioRead(g_hmmio, (HPSTR)&g_wfx, sizeof(g_wfx)); }
	else {
		mmioRead(g_hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
		memset(&g_wfx, 0x00, sizeof(g_wfx)); memcpy(&g_wfx, &pcmwf, sizeof(pcmwf)); g_wfx.cbSize = 0;
	}
	mmioAscend(g_hmmio, &mmckinfo, 0);

	memset(&g_datachunkinfo, 0x00, sizeof(g_datachunkinfo));
	g_datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmret = mmioDescend(g_hmmio, &g_datachunkinfo, &g_riffchunkinfo, MMIO_FINDCHUNK);
	if (mmret != MMSYSERR_NOERROR) return false;

	return true;
}

int main() {
	CSoundCallback callback; XAUDIO2_BUFFER bufinfo; XAUDIO2_VOICE_STATE state;
	unsigned char *buf, *ptr; UINT32 buflen, cnt; LONG readlen; HRESULT hr; DWORD dw;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	do {
		buf = NULL; 
		cnt = 0;
		g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (g_hEvent == NULL) break;


		hr = XAudio2Create(&g_lpXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
		if (FAILED(hr)) break;

		hr = g_lpXAudio->CreateMasteringVoice(&g_lpMasterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
		if (FAILED(hr)) break;
		if (!LoadSound(TEXT("test.wav"))) break;

#ifdef _USE_VOICECALLBACK_
		hr = g_lpXAudio->CreateSourceVoice(&g_lpSourceVoice, &g_wfx, XAUDIO2_VOICE_NOPITCH, XAUDIO2_DEFAULT_FREQ_RATIO, &callback, NULL, NULL);
#else
		hr = g_lpXAudio->CreateSourceVoice(&g_lpSourceVoice, &g_wfx, XAUDIO2_VOICE_NOPITCH | XAUDIO2_VOICE_MUSIC, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
#endif
		if (FAILED(hr)) break;

		buflen = g_wfx.nAvgBytesPerSec / 4;
		buf = new unsigned char[buflen * BUFFERQUEUE_ALLOC];

		ptr = buf; cnt = (cnt + 1) % BUFFERQUEUE_ALLOC;
		readlen = mmioRead(g_hmmio, (HPSTR)ptr, buflen);
		if (readlen <= 0) break;

		memset(&bufinfo, 0x00, sizeof(bufinfo));
		bufinfo.Flags = ((UINT32)readlen >= buflen) ? 0 : XAUDIO2_END_OF_STREAM;
		bufinfo.AudioBytes = readlen;
		bufinfo.pAudioData = ptr;
		bufinfo.PlayBegin = 0;
		bufinfo.PlayLength = readlen / g_wfx.nBlockAlign;

		hr = g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
		if (FAILED(hr)) break;

		hr = g_lpSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
		if (FAILED(hr)) break;

#ifdef _USE_VOICECALLBACK_
		while (dw = WaitForSingleObject(g_hEvent, INFINITE), dw == WAIT_OBJECT_0) {
#else
		while (1) {
#endif
			g_lpSourceVoice->GetState(&state);
			if (state.BuffersQueued == 0) break;
			while (state.BuffersQueued < BUFFERQUEUE_MAX && g_hmmio != NULL) {
				ptr = buf + buflen * cnt; cnt = (cnt + 1) % BUFFERQUEUE_ALLOC;
				readlen = mmioRead(g_hmmio, (HPSTR)ptr, buflen);
				if (readlen <= 0) break;

				bufinfo.Flags = ((UINT32)readlen >= buflen) ? 0 : XAUDIO2_END_OF_STREAM;
				bufinfo.AudioBytes = readlen;
				bufinfo.pAudioData = ptr;
				bufinfo.PlayBegin = 0;
				bufinfo.PlayLength = readlen / g_wfx.nBlockAlign;

				g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
				state.BuffersQueued++;

				if (bufinfo.Flags & XAUDIO2_END_OF_STREAM) {
					mmioClose(g_hmmio, 0);
					g_hmmio = NULL;
				}
			}
#ifndef _USE_VOICECALLBACK_
			Sleep(50);
#endif
		}

	} while (0);

	if (buf != NULL) { delete[] buf; }
	if (g_lpSourceVoice != NULL) { g_lpSourceVoice->DestroyVoice(); }
	if (g_lpMasterVoice != NULL) { g_lpMasterVoice->DestroyVoice(); }
	if (g_lpXAudio != NULL) { g_lpXAudio->Release(); }
	if (g_hmmio != NULL) { mmioClose(g_hmmio, 0); }
	if (g_hEvent != NULL) { CloseHandle(g_hEvent); }

	CoUninitialize();
	return 0;
}