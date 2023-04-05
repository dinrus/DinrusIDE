#include "SSH.h"

namespace РНЦПДинрус {

namespace SSH {
bool sTrace = false;
int  sTraceVerbose = 0;

// SSH diagnostic utilities.

Ткст дайИмя(int тип, int64 ид)
{
	Ткст s;
	switch(тип) {
		case Ssh::SESSION:
			s = "Session";
			break;
		case Ssh::SFTP:
			s = "SFtp";
			break;
		case Ssh::CHANNEL:
			s = "Channel";
			break;
		case Ssh::SCP:
			s = "Scp";
			break;
		case Ssh::EXEC:
			s = "Exec";
			break;
		case Ssh::SHELL:
			s = "Shell";
			break;
		case Ssh::TUNNEL:
			s = "Tunnel";
			break;
		default:
			return "";
	}
	return pick(фмт("SSH: %s, oid: %d: ", s, ид));
}
}

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

// Ssh: SSH objects core class.

static СтатическийСтопор sLoopLock;

bool Ssh::пуск(Врата<>&& фн, bool abortable)
{
	auto делай = [=, &фн]()
	{
		Стопор::Замок __(sLoopLock);

		if(таймаут_ли())
			выведиОш(-1, "Таймаут операции.");

		if(abortable && ssh->status == ABORTED)
			выведиОш(-1, "Операция прервана.");
	
		if(ssh->socket && ssh->socket->ошибка_ли())
			выведиОш(-1, "[Ошибка Сокета]: " << ssh->socket->GetErrorDesc());

		if(!ssh->init)
			ssh->init = иниц();
		
		return !ssh->init || ! фн();
	};
	
	try {
		ssh->status = WORKING;
		ssh->start_time = msecs();
			
		while(делай())	жди();

		ssh->status = IDLE;
	}
	catch(const Ошибка& e) {
		устОш(e.код_, e);
	}
	catch(...) {
		устОш(-1, "Необработанное исключение.");
	}

	return !ошибка_ли();
}

void Ssh::жди()
{
	UpdateClient();
	if(!ssh->socket || !ssh->session)
		return;
	dword q = 0, r = libssh2_session_block_directions(ssh->session);
	if(r & LIBSSH2_SESSION_BLOCK_INBOUND)
		q |= WAIT_READ;
	if(r & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		q |= WAIT_WRITE;
	SocketWaitСобытие we;
	we.добавь(*ssh->socket, q);
	we.жди(ssh->waitstep);
}

void Ssh::выведиОш(int rc, const Ткст& reason)
{
	if(пусто_ли(reason) && ssh && ssh->session) {
		Буфер<char*> libmsg(256, 0);
		rc = libssh2_session_last_error(ssh->session, libmsg, nullptr, 0);
		throw Ошибка(rc, *libmsg);
	}
	else
		throw Ошибка(rc, reason);
}

void Ssh::устОш(int rc, const Ткст& reason)
{
	ssh->status  = FAILED;
	ssh->Ошибка.a = rc;
	ssh->Ошибка.b = reason;
	if(ssh->socket) {
		ssh->socket->ClearAbort();
		ssh->socket->сотриОш();
	}
	LLOG("Failed. Code = " << rc << ", " << reason);
}

int64 Ssh::GetNewId()
{
	static std::atomic<int64> objectid(0);
	return ++objectid;
}

Ssh::Ssh()
{
    ssh.создай();
    ssh->session        = nullptr;
    ssh->socket         = nullptr;
    ssh->init           = false;
    ssh->timeout        = Null;
    ssh->start_time     = 0;
    ssh->waitstep       = 10;
    ssh->chunk_size     = CHUNKSIZE;
    ssh->status         = IDLE;
    ssh->oid            = GetNewId();
    ssh->otype          = CORE;
}

Ssh::~Ssh()
{
}

ИНИЦИАЛИЗАТОР(SSH) {
	libssh2_init(0);
}
ЭКЗИТБЛОК {
	libssh2_exit();
}
}