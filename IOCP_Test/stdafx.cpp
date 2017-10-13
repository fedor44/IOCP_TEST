
// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// IOCP_Test.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"
#include "ClientData.h"

SOCKET g_hSockServer = NULL;
CPtrList g_ptrListClient;
UINT CompletionThread(LPVOID pComPort);


UINT StartServer(LPVOID pParam)
{
	SOCKET hClientSocket = NULL;
	SOCKADDR_IN ServerAddr;
	SOCKADDR_IN ClientAddr;

	memset(&ServerAddr, 0x00, sizeof(SOCKADDR_IN));
	memset(&ClientAddr, 0x00, sizeof(SOCKADDR_IN));

	HANDLE hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if(hCompletionPort == NULL)
	{
		AfxMessageBox("IOCP ���� ����");
		return 10;
	}

	for(int i=0; i<4; i++)
	{
		AfxBeginThread(CompletionThread, (LPVOID)hCompletionPort);
	}

	g_hSockServer = ::WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if(g_hSockServer == INVALID_SOCKET)
	{
		AfxMessageBox("���� ���� ���� ����");
		::CloseHandle(hCompletionPort);
		::ExitProcess(0);
		return 20;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(21000);

	if(::bind(g_hSockServer, (SOCKADDR*)&ServerAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		AfxMessageBox("Bind Error : ��Ʈ�� �̹� �����");
		::CloseHandle(hCompletionPort);
		::ExitProcess(0);
		return 30;
	}

	if(::listen(g_hSockServer, 5) == SOCKET_ERROR)
	{
		AfxMessageBox("listen Error : Ŭ���̾�Ʈ ������ ���� �� ����");
		::CloseHandle(hCompletionPort);
		::ExitProcess(0);
		return 40;
	}

	DWORD dwflag = 0;
	int nAddrSize = sizeof(SOCKADDR_IN);
	DWORD dwReceiveSize = 0;

	while(TRUE)
	{
		hClientSocket = ::accept(g_hSockServer, (SOCKADDR*)&ClientAddr, &nAddrSize);

		if(hClientSocket == INVALID_SOCKET)
			break;

		CClientData* pNewClientData = new CClientData;

		pNewClientData->m_hSocket = hClientSocket;
		memcpy(&(pNewClientData->m_Address), &ClientAddr, nAddrSize);
		pNewClientData->m_IOData.pClientData = (LPVOID)pNewClientData;

		::CreateIoCompletionPort((HANDLE)pNewClientData->m_hSocket, hCompletionPort, (DWORD)&pNewClientData->m_IOData, 0);

		g_ptrListClient.AddTail(pNewClientData);

		dwReceiveSize = 0;
		::WSARecv(pNewClientData->m_hSocket, &pNewClientData->m_IOData.WsaBuffer, 1, &dwReceiveSize, &dwflag, &pNewClientData->m_IOData.Overlapped, NULL);

		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			AfxMessageBox("��Ʈ��ũ ��� ����");
			::CloseHandle(hCompletionPort);
			::ExitProcess(0);
			return 50;
		}
	}

	::CloseHandle(hCompletionPort);
	return 0;
}

UINT CompletionThread(LPVOID pComPort)
{
	CString strTemp = "";
	HANDLE hCompletionPort = (HANDLE)pComPort;
	DWORD dwTransferredSize = 0;
	DWORD dwFlag = 0;

	IO_DATA* pIOData = NULL;
	CClientData* pClientData = NULL;
	LPOVERLAPPED pOverlapped = NULL;

	while(TRUE)
	{
		pIOData = NULL;
		pClientData = NULL;
		pOverlapped = NULL;

		::GetQueuedCompletionStatus(hCompletionPort, &dwTransferredSize, (DWORD*)&pIOData, (LPOVERLAPPED*)&pOverlapped, INFINITE);

		if(pIOData != NULL)
			pClientData = (CClientData*)pIOData->pClientData;
		else
			continue;

		if(dwTransferredSize == 0)
		{
			if(pClientData != NULL)
			{
				::shutdown(pClientData->m_hSocket, SD_BOTH);
				::closesocket(pClientData->m_hSocket);

				POSITION pos = g_ptrListClient.Find(pClientData);
				if(pos != NULL)
					g_ptrListClient.RemoveAt(pos);

				delete pClientData;
				continue;
			}
		}

		if(pClientData != NULL)
		{
			pClientData->m_IOData.WsaBuffer.len = dwTransferredSize;
			::WSASend(pClientData->m_hSocket, &(pClientData->m_IOData.WsaBuffer), 1, NULL, 0, NULL, NULL);

			AfxMessageBox("Ȯ���� ���� ������ ó���� ��� ����");

			memset(pClientData->m_IOData.byBuffer, 0x00, 15000);
			pClientData->m_IOData.WsaBuffer.len = 15000;
			::WSARecv(pClientData->m_hSocket, &(pClientData->m_IOData.WsaBuffer), 1, NULL, &dwFlag, &pClientData->m_IOData.Overlapped, NULL);
		}
	}

	return 0;
}