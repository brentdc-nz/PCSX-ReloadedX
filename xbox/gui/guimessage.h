#ifndef H_CGUIMESSAGE
#define H_CGUIMESSAGE

#include <string>

#define GUI_MSG_WINDOW_INIT    01
#define GUI_MSG_WINDOW_DEINIT  02
#define GUI_MSG_CLICKED        03 // Control has been clicked
#define GUI_MSG_SETFOCUS       04  
#define GUI_MSG_UPDATED        05
#define GUI_MSG_FILENAME       06
#define GUI_MSG_ADD_ITEM       07
#define GUI_MSG_SET_ITEM       10//08
#define GUI_MSG_SET_LABEL      11

class CGUIMessage
{
public:
	CGUIMessage(int iMsg, int iSenderID, int iControlID);
	CGUIMessage(int iMsg, int iSenderID, int iControlID, std::string strParam1);
	CGUIMessage(int iMsg, int iSenderID, int iControlID, int iValue);
	CGUIMessage(int iMsg, int iSenderID, int iControlID, std::string strParam1, int iValue);
	CGUIMessage(int iMsg, int iSenderID, int iControlID, std::string strParam1, std::string strParam2);
	~CGUIMessage();

	int GetMessage() const;
	int GetControlID() const;
	std::string GetStringParam1() const { return m_strParameter1; };
	std::string GetStringParam2() const { return m_strParameter2; };
	int GetIntParam() const { return m_iIntParameter; };

private:
	int m_iMessage;
	int m_iSenderID;
	int m_iControlID;
	std::string m_strParameter1;
	std::string m_strParameter2;
	int m_iIntParameter;
};

#endif //H_CGUIMESSAGE