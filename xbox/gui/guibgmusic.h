#ifndef H_CGUIBGMUSIC
#define H_CGUIBGMUSIC

#include <dmusici.h>

class CGUIBGMusic
{
public:
	CGUIBGMusic();
	~CGUIBGMusic();

	void StartMusic();
	void StopMusic();
	bool IsPlaying() { return m_bPlaying; };
	void Release();

private:
	void Initialize();

	bool m_bInitialized;
	bool m_bDXMusicAllocated;
	bool m_bPlaying;

	IDirectMusicLoader8*		m_pDXMusicLoader;
	IDirectMusicPerformance8*	m_pDXMusicPerformance;
	IDirectMusicSegment8*		m_pDXSoundSegment;
};

#endif //H_CGUIBGMUSIC