#include "SSH.h"

namespace РНЦПДинрус {

int SshExec::выполни(const Ткст& cmd, Ткст& out, Ткст& err)
{
	if(RequestExec(cmd)) {
		ReadStdOut(out);
		ReadStdErr(err);
		if(Shut(ошибка_ли() ? GetErrorDesc() : Null))
			return  дайКодВыхода();
	}
	return дайОш();
}

void SshExec::ReadStdOut(Ткст& out)
{
	Ткст s;
	while(!(s = дай(ssh->chunk_size)).пустой())
		out.конкат(s);
}

void SshExec::ReadStdErr(Ткст& err)
{
	Ткст s;
	while(!(s = GetStdErr(ssh->chunk_size)).пустой())
		err.конкат(s);
}

int SshExecute(SshSession& session, const Ткст& cmd, Ткст& out, Ткст& err)
{
	return SshExec(session).выполни(cmd, out, err);
}

int SshExecute(SshSession& session, const Ткст& cmd, Ткст& out)
{
	Ткст err;
	int rc = SshExec(session).выполни(cmd, out, err);
	if(!пусто_ли(err))
		out.конкат(err);
	return rc;
}

Ткст SshExecute(SshSession& session, const Ткст& cmd)
{
	Ткст out, err;
	return SshExecute(session, cmd, out, err) ? Ткст::дайПроц(): out;
}

}