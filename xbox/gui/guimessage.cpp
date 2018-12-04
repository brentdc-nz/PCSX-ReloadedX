#include "guimessage.h"

CGUIMessage::CGUIMessage(int iMsg, int iSenderID, int iControlID)
{
	m_iMessage = iMsg;
	m_iSenderID = iSenderID;
	m_iControlID = iControlID;
	m_strParameter1 = "";
	m_strParameter2 = "";
	m_iIntParameter = 0;
}

CGUIMessage::CGUIMessage(int iMsg, int iSenderID, int iControlID, std::string strParam1)
{
	m_iMessage = iMsg;
	m_iSenderID = iSenderID;
	m_iControlID = iControlID;
	m_strParameter1 = strParam1;
	m_strParameter2 = "";
	m_iIntParameter = 0;
}

CGUIMessage::CGUIMessage(int iMsg, int iSenderID, int iControlID, int iValue)
{
	m_iMessage = iMsg;
	m_iSenderID = iSenderID;
	m_iControlID = iControlID;
	m_strParameter1 = "";
	m_strParameter2 = "";
	m_iIntParameter = iValue;
}

CGUIMessage::CGUIMessage(int iMsg, int iSenderID, int iControlID, std::string strParam1, int iValue)
{
	m_iMessage = iMsg;
	m_iSenderID = iSenderID;
	m_iControlID = iControlID;
	m_strParameter1 = strParam1;
	m_strParameter2 = "";
	m_iIntParameter = iValue;
}

CGUIMessage::CGUIMessage(int iMsg, int iSenderID, int iControlID, std::string strParam1, std::string strParam2)
{
	m_iMessage = iMsg;
	m_iSenderID = iSenderID;
	m_iControlID = iControlID;
	m_strParameter1 = strParam1;
	m_strParameter2 = strParam2;
	m_iIntParameter = 0;
}

CGUIMessage::~CGUIMessage()
{
}

int CGUIMessage::GetMessage() const
{
	return m_iMessage;
}

int CGUIMessage::GetControlID() const
{
	return m_iControlID;
}
