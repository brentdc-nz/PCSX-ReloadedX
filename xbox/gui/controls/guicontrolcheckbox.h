#ifndef H_CGUICONTROLCHECKBOX
#define H_CGUICONTROLCHECKBOX

#include "..\guicontrol.h"
#include "..\guifont.h"
#include <string>

#include "guicontrolimage.h"
#include "guicontrollabel.h"

class CGUIControlCheckbox : public CGUIControl
{
public:
	CGUIControlCheckbox(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, std::string strText, DWORD dwColor, unsigned int iSize, 
		                std::string strImageFocus, int iImgXOffset, std::string strImage, std::string strImageSelected, int ImgWidth, int ImgHeight);
	~CGUIControlCheckbox();
	
	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	virtual bool OnKey(int iKey);

	virtual void Selected(bool bSelected);
	virtual bool IsSelcted();
	
	std::string GetFont() { return m_label.GetFont(); };
	DWORD GetColor() { return  m_label.GetColor(); };
	unsigned int GetSize() { return m_label.GetSize(); };
	int GetImageXOffset() { return m_iImageOffsetX; };
	int GetImageWidth() { return m_imgStatusOn.GetWidth(); };
	int GetImageHeight() { return m_imgStatusOn.GetHeight(); };
	std::string GetFocusImage() { return m_imgFocused.GetImageFilename(); };
	std::string GetTexture() { return m_imgStatus.GetImageFilename(); };
	std::string GetTextureSlected() { return m_imgStatusOn.GetImageFilename(); };

private:
	int m_iImageOffsetX;
	CGUIControlLabel m_label;
	CGUIControlImage m_imgFocused;
	CGUIControlImage m_imgStatus;
	CGUIControlImage m_imgStatusOn;
};

#endif //H_CGUICONTROLCHECKBOX