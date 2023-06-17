#include "SSH.h"

цел SshExec::выполни(const Ткст& cmd, Ткст& out, Ткст& err)
{
	if(RequestExec(cmd)) {
		ReadStdOut(out);
		ReadStdErr(err);
		if(Shut(ошибка_ли() ? GetErrorDesc() : Null))
			return  дайКодВыхода();
	}
	return дайОш();
}

проц SshExec::ReadStdOut(Ткст& out)
{
	Ткст s;
	while(!(s = дай(ssh->chunk_size)).пустой())
		out.кат(s);
}

проц SshExec::ReadStdErr(Ткст& err)
{
	Ткст s;
	while(!(s = GetStdErr(ssh->chunk_size)).пустой())
		err.кат(s);
}

цел SshExecute(SshSession& session, const Ткст& cmd, Ткст& out, Ткст& err)
{
	return SshExec(session).выполни(cmd, out, err);
}

цел SshExecute(SshSession& session, const Ткст& cmd, Ткст& out)
{
	Ткст err;
	цел rc = SshExec(session).выполни(cmd, out, err);
	if(!пусто_ли(err))
		out.кат(err);
	return rc;
}

Ткст SshExecute(SshSession& session, const Ткст& cmd)
{
	Ткст out, err;
	return SshExecute(session, cmd, out, err) ? Ткст::дайПроц(): out;
}
