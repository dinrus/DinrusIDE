//#include <process.h>
#include "ReadDirectoryChanges.h"
#include "ReadDirectoryChangesPrivate.h"

using namespace ReadDirectoryChangesPrivate;

///////////////////////////////////////////////////////////////////////////
// CReadDirectoryChanges

CReadDirectoryChanges::CReadDirectoryChanges()
	: m_Notifications()
{
	m_hThread	= NULL;
	m_dwThreadId= 0;
	m_pServer	= new CReadChangesServer(this);
}

CReadDirectoryChanges::~CReadDirectoryChanges()
{
	Terminate();
	delete m_pServer;
}

void CReadDirectoryChanges::Init()
{
	//
	// Kick off the worker thread, which will be
	// managed by CReadChangesServer.
	//
	m_hThread = (void*)_beginthreadex(NULL,
		0,
		CReadChangesServer::ThreadStartProc,
		m_pServer,
		0,
		&m_dwThreadId
		);
}

void CReadDirectoryChanges::Terminate()
{
	if (m_hThread)
	{
		::QueueUserAPC(CReadChangesServer::TerminateProc, m_hThread, (ULONG_PTR)m_pServer);
		::WaitForSingleObjectEx(m_hThread, 10000, true);
		::CloseHandle(m_hThread);

		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}

void CReadDirectoryChanges::AddDirectory( LPCTSTR szDirectory, BOOL bWatchSubtree, dword dwNotifyFilter, dword dwBufferSize )
{
	if (!m_hThread)
		Init();

	CReadChangesRequest* pRequest = new CReadChangesRequest(m_pServer, szDirectory, bWatchSubtree, dwNotifyFilter, dwBufferSize);
	QueueUserAPC(CReadChangesServer::AddDirectoryProc, m_hThread, (ULONG_PTR)pRequest);
}

void CReadDirectoryChanges::Push(dword dwAction, std::wstring& wstrFilename)
{
	TDirectoryChangeNotification dirChangeNotif = TDirectoryChangeNotification(dwAction, wstrFilename);
	m_Notifications.push(dirChangeNotif);
}

bool  CReadDirectoryChanges::Pop(dword& dwAction, std::wstring& wstrFilename)
{
	TDirectoryChangeNotification pair;
	if (!m_Notifications.pop(pair))
		return false;

	dwAction = pair.first;
	wstrFilename = pair.second;

	return true;
}
