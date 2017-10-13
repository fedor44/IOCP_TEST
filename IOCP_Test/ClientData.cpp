#include "StdAfx.h"
#include "ClientData.h"

CClientData::CClientData(void)
{
	m_hSocket = NULL;
	memset(&m_Address, 0x00, sizeof(m_Address));
	memset(&m_IOData.Overlapped, 0x00, sizeof(OVERLAPPED));
	memset(m_IOData.byBuffer, 0x00, 15000);

	m_IOData.WsaBuffer.buf = (char*)m_IOData.byBuffer;
	m_IOData.WsaBuffer.len = 15000;
}

CClientData::~CClientData(void)
{
}
