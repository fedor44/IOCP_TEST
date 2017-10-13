#if _MSC_VER > 1000
#pragma once
#endif

typedef struct _IO_DATA
{
	WSAOVERLAPPED Overlapped;
	BYTE byBuffer[15000];
	WSABUF WsaBuffer;
	LPVOID pClientData;
}IO_DATA;

class CClientData
{
public:
	CClientData(void);
	~CClientData(void);

public:
	IO_DATA m_IOData;
	SOCKET m_hSocket;
	SOCKADDR_IN m_Address;
};
