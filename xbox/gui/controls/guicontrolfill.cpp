#include "guicontrolfill.h"
#include "..\xboxgui.h"

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

CGUIControlFill::CGUIControlFill(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwColor) 
: CGUIControl(iControlID, iWindowID, iPosX, iPosY, iWidth, iHeight)
{
	m_dwColor = dwColor;
}

CGUIControlFill::~CGUIControlFill()
{
}

bool CGUIControlFill::AllocateResources()
{
	if(m_bResoucesAlocated)
		return true;

	m_pd3dDevice = g_XBoxGUI.GetD3DDevice();

	if(!m_pd3dDevice)
		return false;

	// Create the vertex buffer from our device
	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOM_FILL_VERTEX), 0, CUSTOMFVF, D3DPOOL_DEFAULT, &m_pVertexBuffer);

	m_bResoucesAlocated = true;

	return CGUIControl::AllocateResources();
}

void CGUIControlFill::FreeResources()
{
	m_bResoucesAlocated = false;
    m_pVertexBuffer->Release();
}

void CGUIControlFill::Render()
{
	VOID* pVertices;

	CUSTOM_FILL_VERTEX cvVertices[] =
	{
		{ (FLOAT)m_iPosX, (FLOAT)m_iPosY, 0.5f, 1.0f, m_dwColor},
		{ (FLOAT)m_iPosX+m_iWidth, (FLOAT)m_iPosY, 0.5f, 1.0f, m_dwColor},
		{ (FLOAT)m_iPosX, (FLOAT)m_iHeight+m_iPosY, 0.5f, 1.0f, m_dwColor},
		{ (FLOAT)m_iPosX+(FLOAT)m_iWidth, (FLOAT)m_iPosY+m_iHeight, 0.5f, 1.0f, m_dwColor},
	};

	// Get a pointer to the vertex buffer vertices and lock the vertex buffer
	m_pVertexBuffer->Lock(0, sizeof(cvVertices), (BYTE**)&pVertices, 0);

	// Copy our stored vertices values into the vertex buffer
	memcpy(pVertices, cvVertices, sizeof(cvVertices));

	// Unlock the vertex buffer
	m_pVertexBuffer->Unlock();

	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, sizeof(CUSTOM_FILL_VERTEX));
	m_pd3dDevice->SetVertexShader(CUSTOMFVF);

	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}