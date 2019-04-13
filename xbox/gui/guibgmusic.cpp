#include "guibgmusic.h"

#define BG_MUSIC_FILE "D:\\assets\\bgmusic.wav"

CGUIBGMusic::CGUIBGMusic()
{
	m_bInitialized = false;
	m_bDXMusicAllocated = false;
	m_bPlaying = false;

	m_pDXMusicLoader = NULL;
	m_pDXMusicPerformance = NULL;
	m_pDXSoundSegment = NULL;
}

CGUIBGMusic::~CGUIBGMusic()
{
}

void CGUIBGMusic::Initialize()
{
	if(m_bInitialized)
		return;

	// Initialize DirectMusic
	if(!m_bDXMusicAllocated)
	{
		// Docs say this is done once per XBE run
		// seems strange as it's released aswell
		IDirectMusicHeap* pNormalHeap;
		DirectMusicCreateDefaultHeap(&pNormalHeap);

		IDirectMusicHeap* pPhysicalHeap;
		DirectMusicCreateDefaultPhysicalHeap(&pPhysicalHeap);

		DirectMusicInitializeEx(pNormalHeap, pPhysicalHeap, &DirectMusicDefaultFactory);

		pNormalHeap->Release();
		pPhysicalHeap->Release();

		m_bDXMusicAllocated = true;
	}

	// Create loader object
	DirectMusicCreateInstance(CLSID_DirectMusicLoader, NULL,
                              IID_IDirectMusicLoader8, (VOID**)&m_pDXMusicLoader);

	// Create performance object
	DirectMusicCreateInstance(CLSID_DirectMusicPerformance, NULL,
                              IID_IDirectMusicPerformance8, (VOID**)&m_pDXMusicPerformance);

	m_pDXMusicPerformance->InitAudioX(DMUS_APATH_SHARED_STEREO, 64, 0, NULL);	

	m_pDXMusicLoader->LoadObjectFromFile(CLSID_DirectMusicSegment, IID_IDirectMusicSegment8,
										 BG_MUSIC_FILE, (VOID**)&m_pDXSoundSegment);

	m_bInitialized = true;
}

void CGUIBGMusic::StartMusic()
{
	if(!m_bInitialized)
		Initialize();

	m_pDXSoundSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);

	m_pDXMusicPerformance->PlaySegmentEx(m_pDXSoundSegment, NULL, NULL, DMUS_SEGF_SECONDARY, 
		                                 0, NULL, NULL, NULL);

	m_bPlaying = true;
}

void CGUIBGMusic::StopMusic()
{
	if(m_bInitialized)
		m_pDXMusicPerformance->StopEx(NULL, 0, NULL);

	m_bPlaying = false;
}

void CGUIBGMusic::Release()
{
	if(!m_bInitialized)
		return;

	if(IsPlaying())
		StopMusic();

	if(m_pDXMusicLoader)
		m_pDXMusicLoader->Release();

	if(m_pDXMusicPerformance)
		m_pDXMusicPerformance->Release();

	if(m_pDXSoundSegment)
		m_pDXSoundSegment->Release();

	m_pDXMusicLoader = NULL;
	m_pDXMusicPerformance = NULL;
	m_pDXSoundSegment = NULL;

	m_bInitialized = false;
}