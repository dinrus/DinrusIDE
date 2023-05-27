#include "SSH.h"

namespace ДинрусРНЦП {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define VLOG(x)       if(SSH::sTraceVerbose) LLOG(x);
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

бул SshChannel::иниц()
{
	ПРОВЕРЬ(!открыт());

	LIBSSH2_CHANNEL *ch = libssh2_channel_open_session(ssh->session);
	if(!ch && !WouldBlock())
		выведиОш(-1);
	if(ch) {
		channel = сделайОдин<LIBSSH2_CHANNEL*>(ch);
		LLOG("Открыт новый канал.");
	}
	return ch;
}

проц SshChannel::выход()
{
	if(!channel)
		return;

	пуск([=]() mutable {
		цел rc = libssh2_channel_free(*channel);
		if(!WouldBlock(rc) && rc < 0)
			выведиОш(rc);
		if(!rc) {
			ssh->init = false;
			channel.очисть();
			LLOG("Channel succesfully freed.");
		}
		return !rc;
	}, false);
}

бул SshChannel::открой()
{
	if(открыт())
		открой();
	return пуск([=]() mutable { return иниц(); });
}

бул SshChannel::открой()
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_close(*channel);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(!rc) LLOG("Channel close message is sent to the server.");
		return !rc;
	});
}

бул SshChannel::WaitClose()
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_wait_closed(*channel);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(!rc)	LLOG("Channel close message is acknowledged by the server.");
		return !rc;
	});
}

бул SshChannel::Request(const Ткст& request, const Ткст& params)
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_process_startup(
			*channel,
			request,
			request.дайДлину(),
			params.дайДлину() ? ~params : nullptr,
			params.дайДлину()
		);
		if(!WouldBlock(rc) && rc < 0)
			выведиОш(rc);
		if(!rc)
			LLOG("\"" << request << "\" request (params: " << params << ") is successful.");
		return !rc;
	});
}

бул SshChannel::RequestTerminal(const Ткст& term, цел width, цел height, const Ткст& modes)
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_request_pty_ex(
			*channel,
			~term,
			term.дайДлину(),
			~modes,
			modes.дайДлину(),
			width,
			height,
			LIBSSH2_TERM_WIDTH_PX,
			LIBSSH2_TERM_HEIGHT_PX
		);
		if(!WouldBlock(rc) && rc < 0)
			выведиОш(rc);
		if(!rc)
			LLOG("Terminal (" << term << ") [W:" << width << ", H:" << height << "] opened.");
		return !rc;
	});
}

бул SshChannel::SetEnv(const Ткст& ПЕРЕМЕННАЯ, const Ткст& значение)
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_setenv(*channel, ПЕРЕМЕННАЯ, значение);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(!rc)	LLOG("систСреда ПЕРЕМЕННАЯ '" << ПЕРЕМЕННАЯ << "' set to " << значение);
		return !rc;
	});
}

бул SshChannel::PutEof()
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_send_eof(*channel);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(!rc)	LLOG("EOF message is sent to the server.");
		return !rc;
	});
}

бул SshChannel::GetEof()
{
	return пуск([=]() mutable {
		цел rc = libssh2_channel_wait_eof(*channel);
		if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
		if(!rc) LLOG("EOF message is acknowledged by the server.");;
		return !rc;
	});
}

бул SshChannel::кф_ли()
{
	бул b = false;
	INTERLOCKED
	{
		b = libssh2_channel_eof(*channel) != 0;
	}
	if(b)
		LLOG("EOF received.");
	return b;
}

бул SshChannel::SetTerminalSize(цел width, цел height)
{
	return пуск([=]() mutable { return SetPtySz(width, height) >= 0; });
}

цел SshChannel::SetPtySz(цел w, цел h)
{
	цел rc = libssh2_channel_request_pty_size(*channel, w, h);
	if(!WouldBlock(rc) && rc < 0) rc = 1;
	if(rc == 1)	LLOG("Warning: Couldn't set terminal size!");
	if(rc == 0)	LLOG("Terminal size adjusted. [W:" << w << ", H:" << h << "]");
	return rc;
}

бул SshChannel::SetReadWindowSize(бцел size, бул force)
{
	return пуск([=]() mutable { return SetWndSz(size, force); });
}

бул SshChannel::SetWndSz(бцел size, бул force)
{
	цел rc = libssh2_channel_receive_window_adjust2(*channel, size, (unsigned char) force, nullptr);
	if(!WouldBlock(rc) && rc < 0) выведиОш(rc);
	if(!rc) LLOG(фмт("Receive window size set is to %d.", какТкст(size)));
	return !rc;
}

цел SshChannel::дайКодВыхода()
{
	цел rc = 0;
	INTERLOCKED
	{
		rc = libssh2_channel_get_exit_status(*channel);
	}
	LLOG("выход код_: " << rc);
	return rc;
}

Ткст SshChannel::GetExitSignal()
{
	char *sig = (char *)"none";
	т_мера len = 0;
	Ткст s;
	INTERLOCKED
	{
		libssh2_channel_get_exit_signal(*channel, &sig, &len, nullptr, nullptr, nullptr, nullptr);
	}
	s.уст(sig, len);
	LLOG("выход signal: " << s);
	return s;
}

цел SshChannel::дай(ук укз, цел size, цел sid)
{
	done = 0;
	пуск([=]() mutable {
		while(done < size && !кф_ли() && !таймаут_ли()) {
			цел rc = читай(укз, size, sid);
			if(rc < 0) return false;
			if(!rc) break;
			UpdateClient();
		}
		return true;
	});
	return GetDone();
}

Ткст SshChannel::дай(цел size, цел sid)
{
	ТкстБуф sb(size);
	цел len = дай(~sb, size, sid);
	sb.устСчёт(len);
	return pick(Ткст(sb));
}

Ткст SshChannel::дайСтроку(цел maxlen, цел sid)
{
	done = 0;
	Ткст line;
	пуск([=, &line]{
		бул eol = false;
		do {
			цел c = читай(sid);
			if(c == -1)
				break;
			ssh->start_time = msecs();
			if(c == '\r')
				continue;
			if(line.дайДлину() >= maxlen)
				line = Null;
			eol = c == '\n';
			if(!eol) {
				line.кат(c);
				done++;
			}
		}
		while(!eol && !кф_ли() && !таймаут_ли());
		return eol || кф_ли();
	});
	return line;
}

цел SshChannel::помести(const ук укз, цел size, цел sid)
{
	done = 0;
	пуск([=]() mutable {
		while(done < size && !кф_ли() && !таймаут_ли()) {
			цел rc = пиши(укз, size, sid);
			if(rc < 0) return false;
			if(!rc) break;
			UpdateClient();
		}
		return true;
	});
	return GetDone();
}

цел SshChannel::читай(ук укз, цел size, цел sid)
{
	цел sz = мин(size - done, ssh->chunk_size);

	цел rc = static_cast<цел>(
		libssh2_channel_read_ex(*channel, sid, (char*) укз + done, т_мера(sz))
		);
	if(rc < 0 && !WouldBlock(rc)) {
		выведиОш(rc);
	}
	else
	if(rc > 0) {
		done += rc;
		ssh->start_time = msecs();
		VLOG("читай stream #" << sid << ": " << rc << " bytes read.");
	}
	return rc;
}

цел SshChannel::читай(цел sid)
{
	char c;
	done = 0;
	return читай(&c, 1, sid) == 1 ? цел(c) : -1;
}

цел SshChannel::пиши(const ук укз, цел size, цел sid)
{
	цел sz = мин(size - done, ssh->chunk_size);

	цел rc = static_cast<цел>(
		libssh2_channel_write_ex(*channel, sid, (const char*) укз + done, т_мера(sz))
		);
	if(rc < 0 && !WouldBlock(rc)) {
		выведиОш(rc);
	}
	else
	if(rc > 0) {
		done += rc;
		ssh->start_time = msecs();
		VLOG("пиши stream #" << sid << ": " << rc << " bytes written.");
	}
	return rc;
}

бул SshChannel::пиши(char c, цел sid)
{
	done = 0;
	return пиши(&c, 1, sid) == 1;
}

бцел SshChannel::СобытиеWait(цел fd, бцел events, цел tv)
{
	SocketWaitСобытие we;
	we.добавь(fd, events);
	return we.жди(tv) > 0 ? we[0] : 0;
}

бул SshChannel::обработайСобытия(Ткст& input)
{
	Буфер<char> буфер(ssh->chunk_size, 0);

	return пуск([=, &буфер, &input]{
		done = 0;
		цел len = читай(буфер, ssh->chunk_size);
		ReadWrite(input, буфер, len);
		while(!input.пустой() && InProgress()) {
			done = 0;
			len = пиши(~input, input.дайДлину());
			if(len <= 0)
				break;
			input.удали(0, len);
		}
		return кф_ли();
	});
}


бул SshChannel::Shut(const Ткст& msg, бул nowait)
{
	бул eof = false;
	if(PutEof() && !nowait)
		eof = GetEof();
	if(открой() && eof)
		WaitClose();
	if(!пусто_ли(msg))
		устОш(-1, msg);
	return !ошибка_ли();
}

SshChannel::SshChannel(SshSession& session)
: done(0)
{
	ssh->otype		= CHANNEL;
	ssh->session	= session.дайУк();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->whenwait	= прокси(session.WhenWait);
}

SshChannel::~SshChannel()
{
	выход();
}
}