#ifndef H_CGUIWINDOW
#define H_CGUIWINDOW

#include "guicontrol.h"
#include "..\tinyxml\tinyxml.h"
#include <vector>

class CGUIWindow
{
public:
	CGUIWindow(int iWindowID, std::string strXMLFile);
	~CGUIWindow();

	int GetID() {return m_iID;};
	void SetID(int iID) {m_iID = iID; };
	
	virtual bool IsDialog() const { return false; };
	void Initialize();
	virtual bool LoadXML(std::string strXMLFile);
	virtual bool AllocateResources();
	virtual void FreeResources(bool bClearControls = false);
	void ClearAll();
	virtual bool OnKey(int iKey);
	virtual bool OnBack();
	virtual bool OnMessage(CGUIMessage message);
	virtual void Render();

	CGUIControl* GetControl(int iControlID);
	void InsertControl(CGUIControl* pControl);
	void RemoveControl(int iID);
	CGUIControl* GetFocusedControl();

protected:
	void SaveControlStates();
	void RestoreControlStates();
	void LoadControl(TiXmlElement* pControl);
	
	int m_iLastControlID;
	std::string m_strXMLFile;
	bool m_bLoaded; // True if XML file is loaded
	bool m_bAllocated; // True if resouces have been allocated
	int m_iID;
	int m_iDefaultCOntrolID;
	std::vector <CGUIControl*> m_vecControls;
};

#endif //H_CGUIWINDOW
