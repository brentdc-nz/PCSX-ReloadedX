#ifndef H_CGUICONTROLSPIN
#define H_CGUICONTROLSPIN

#include "..\guicontrol.h"
#include "guicontrollabel.h"
#include "guicontrolimage.h"
#include <vector>

enum SpinDirection
{ 
	SPIN_BUTTON_DOWN = 1,
	SPIN_BUTTON_UP
};

class CGUIControlSpin : public CGUIControl
{
public:
	CGUIControlSpin(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, unsigned int iSize, std::string strText, DWORD dwColor, std::string strImageFocus, std::string strSpinUp, std::string strSpinUpFocus, std::string strSpinDown
		            , std::string strSpinDownFocus, int iSpinWidth, int iSpinHeight);
	~CGUIControlSpin();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	virtual bool OnKey(int iKey);

	std::string GetFont() { return m_label.GetFont(); };
	int GetFontSize() { return m_label.GetSize(); };
	DWORD GetColor() { return m_label.GetColor(); };
	std::string GetFocusImage() { return m_imgFocused.GetImageFilename(); };
	int GetSpinWidth() { return m_ImgScrollUp.GetWidth(); };
	int GetSpinHeight() { return m_ImgScrollUp.GetHeight(); };
	std::string GetSpinUp() { return m_ImgScrollUp.GetImageFilename(); };
	std::string GetSpinUpFocus() { return  m_ImgScrollUpFocus.GetImageFilename(); };
	std::string GetSpinDown() { return  m_ImgScrollDown.GetImageFilename(); };
	std::string GetSpinDownFocus() { return  m_ImgScrollDownFocus.GetImageFilename(); };

protected:
	virtual void OnLeft();
	virtual void OnRight();

	int m_iButtonSelect;

	CGUIControlLabel m_label;
	CGUIControlLabel m_labelValue;

	CGUIControlImage m_imgFocused;
	CGUIControlImage m_ImgScrollUp;
	CGUIControlImage m_ImgScrollUpFocus;
	CGUIControlImage m_ImgScrollDown;
	CGUIControlImage m_ImgScrollDownFocus;
};

//===========================================

class CGUIControlSpinInt : public CGUIControlSpin
{
public:
	CGUIControlSpinInt(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, unsigned int iSize, std::string strText, DWORD dwColor, std::string strImageFocus 
		               ,std::string strSpinUp, std::string strSpinUpFocus, std::string strSpinDown, std::string strSpinDownFocus, int iSpinWidth, int iSpinHeight);
	~CGUIControlSpinInt();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	virtual bool OnMessage(CGUIMessage message);
	virtual bool OnKey(int iKey);

	int GetSelectedValue();

	class COptionsInt
	{
	public:
		COptionsInt(int iValue, std::string strDisplayText){ m_iValue = iValue; m_strDisplayText = strDisplayText; };
		~COptionsInt(){};

		int GetValue() { return m_iValue; };
		std::string GetDisplayText() { return m_strDisplayText; };

	private:
		int m_iValue;
		std::string m_strDisplayText;
	};

protected:
	int m_iValue;
	std::vector <COptionsInt*> m_vecOptions;
};

//===========================================

class CGUIControlSpinString : public CGUIControlSpin
{
public:
	CGUIControlSpinString(int iControlID, int iWindowID, int iPosX, int iPosY, int iWidth, int iHeight, std::string strFont, unsigned int iSize, std::string strText, DWORD dwColor, std::string strImageFocus 
		                  ,std::string strSpinUp, std::string strSpinUpFocus, std::string strSpinDown, std::string strSpinDownFocus, int iSpinWidth, int iSpinHeight);
	~CGUIControlSpinString();

	virtual bool AllocateResources();
	virtual void FreeResources();
	virtual void Render();
	virtual bool OnMessage(CGUIMessage message);
	virtual bool OnKey(int iKey);

	std::string GetSelectedValue();

	class COptionsString
	{
	public:
		COptionsString(std::string strValue, std::string strDisplayText){ m_strValue = strValue; m_strDisplayText = strDisplayText; };
		~COptionsString(){};

		std::string GetValue() { return m_strValue; };
		std::string GetDisplayText() { return m_strDisplayText; };

	private:
		std::string m_strValue;
		std::string m_strDisplayText;
	};

protected:
	int m_iValue;
	std::vector <COptionsString*> m_vecOptions;
};

#endif //H_CGUICONTROLSPIN