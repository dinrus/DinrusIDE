#include "SSH.h"

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

бул SshShell::пуск(цел mode_, const Ткст& terminal, размер pagesize, const Ткст& tmodes)
{
	mode  = mode_;
	psize = pagesize;
	
	if(RequestTerminal(terminal, psize, tmodes) && х11Иниц() && RequestShell() && консИниц())
		обработайСобытия(queue);
	return Shut(ошибка_ли() ? GetErrorDesc() : Null);
}

проц SshShell::ReadWrite(Ткст& in, кук out, цел out_len)
{
	switch(mode) {
		case GENERIC: {
			if(out_len > 0)
				WhenOutput(out, out_len);
#ifdef PLATFORM_POSIX
			if(xenabled)
				х11Цикл();
#endif
			WhenInput();
			break;
		}
		case CONSOLE: {
			if(out_len > 0)
				консПиши(out, out_len);
#ifdef PLATFORM_POSIX
			if(xenabled)
				х11Цикл();
			консЧмтай();
			#if defined(PLATFORM_MACOS)
			// We are using sigtimedwait on POSIX-compliant systems.
			// Unfortunately MacOS didn't implement it. This is a simple workaround for MacOS.
			// It relies on ioctl, which is implemented on MacOS.
			размер sz = GetConsolePageSize();
			resized = !пусто_ли(sz) && sz != psize;
			if(resized)
				LLOG("Window size changed.");
			#else
			// We need to catch the WINCH signal. To this end we'll use a POSIX compliant kernel
			// ФУНКЦИЯ: sigtimedwait. To speed up, we'll simply poll for the monitored event.
			sigset_t set;
			sigemptyset(&set);
			sigaddset(&set, SIGWINCH);
			sigprocmask(SIG_BLOCK, &set, nullptr);

			struct timespec timeout;
			обнули(timeout); // Instead of waiting, we simply poll.

			auto rc = sigtimedwait(&set, nullptr, &timeout);
			if(rc < 0 && errno != EAGAIN)
				выведиОш(-1, "sigtimedwait() failed.");
			if(rc > 0)
				LLOG("SIGWINCH received.");
			resized = rc > 0;
			#endif
#elif PLATFORM_WIN32
			// This part is a little bit tricky. We need to handle Windows console events here.
			// But we cannot simply ignore the events we don't look for. We need to actively
			// remove them from the event queue. Otherwise they'll cause erratic behaviour, and
			// a lot of head ache. Thus to фильтр out these unwanted events, or the event's that
			// we don't want to get in our way, we'll first подбери at the console event queue to
			// see if they met our criteria and remove them one by one as we encounter, using
			// the ReadConsoleInput method.

			auto rc = WaitForSingleObject(stdinput, ssh->waitstep);
			switch(rc) {
				case WAIT_OBJECT_0:
					break;
				case WAIT_TIMEOUT:
				case WAIT_ABANDONED:
					return;
				default:
					выведиОш(-1, "WaitForSingleObject() failed.");
			}

			DWORD n = 0;
			INPUT_RECORD ir[1];

			if(!PeekConsoleInput(stdinput, ir, 1, &n))
				выведиОш(-1, "Unable to подбери console input events.");
			if(n) {
				switch(ir[0].СобытиеType) {
					case KEY_EVENT:
						// Ignore ключ-ups.
						if(!ir[0].Событие.KeyСобытие.bKeyDown)
							break;
						консЧмтай();
						return;
					case WINDOW_BUFFER_SIZE_EVENT:
						LLOG("WINDOW_BUFFER_SIZE_EVENT received.");
						resized = true;
						break;
					case MENU_EVENT:
					case MOUSE_EVENT:
					case FOCUS_EVENT:
						break;
					default:
						выведиОш(-1, "Unknown console event тип encountered.");
				}
				if(!ReadConsoleInput(stdinput, ir, 1, &n))
					выведиОш(-1, "Unable to фильтр console input events.");
			}
#endif
			break;
		}
		default:
			НИКОГДА();
	}
	if(resized)
		Resize();
}

проц SshShell::Resize()
{
	if(mode == CONSOLE)
		PageSize(GetConsolePageSize());

	цел n = 0;
	do {
		n = SetPtySz(psize);
		if(n < 0)
			жди();
	}
	while(!таймаут_ли() && !кф_ли() && n < 0);
	resized = false;
}

бул SshShell::консИниц()
{
	if(mode != CONSOLE)
		return true;
	
	return Ssh::пуск([=]() mutable {
#ifdef PLATFORM_WIN32
		stdinput = GetStdHandle(STD_INPUT_HANDLE);
		if(!stdinput)
			выведиОш(-1, "Unable to obtain a handle for stdin.");
		stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
		if(!stdoutput)
			выведиОш(-1, "Unable to obtain a handle for stdout.");
#endif
		ConsoleRawMode();
		return true;
	});
}

#ifdef PLATFORM_POSIX
проц SshShell::консЧмтай()
{
	if(!СобытиеWait(STDIN_FILENO, WAIT_READ, 0))
		return;
	Буфер<сим> буфер(ssh->chunk_size);
	auto n = read(STDIN_FILENO, буфер, т_мера(ssh->chunk_size));
	if(n > 0)
		Send(Ткст(буфер, n));
	else
	if(n == -1 && errno != EAGAIN)
		выведиОш(-1, "Couldn't read input from console.");
}

проц SshShell::консПиши(кук буфер, цел len)
{
	if(!СобытиеWait(STDOUT_FILENO, WAIT_WRITE, 0))
		return;
	auto n = write(STDOUT_FILENO, буфер, т_мера(len));
	if(n == -1 && errno != EAGAIN)
		выведиОш(-1, "Couldn't write output to console.");
}

проц SshShell::ConsoleRawMode(бул b)
{
	if(!channel || mode != CONSOLE)
		return;

	if(b) {
		termios nflags;
		обнули(nflags);
		обнули(tflags);
		tcgetattr(STDIN_FILENO, &nflags);
		tflags = nflags;
		cfmakeraw(&nflags);
		tcsetattr(STDIN_FILENO, TCSANOW, &nflags);
	}
	else
	if(rawmode)
		tcsetattr(STDIN_FILENO, TCSANOW, &tflags);
	rawmode = b;
}

размер SshShell::GetConsolePageSize()
{
	winsize wsz;
	обнули(wsz);
	if(ioctl(STDIN_FILENO, TIOCGWINSZ, &wsz) == 0)
		return размер(wsz.ws_col, wsz.ws_row);
	LLOG("Warning: ioctl() failed. Couldn't read local terminal page size.");
	return Null;
}

#elif PLATFORM_WIN32

проц SshShell::консЧмтай()
{
	DWORD n = 0;
	const цел RBUFSIZE = 1024 * 16;
	Буфер<сим> буфер(RBUFSIZE);
	if(!ReadConsole(stdinput, буфер, RBUFSIZE, &n, nullptr))
		выведиОш(-1, "Couldn't read input from console.");
	if(n > 0)
		Send(Ткст(буфер, n));
}

проц SshShell::консПиши(кук буфер, цел len)
{
	DWORD n = 0;
	if(!WriteConsole(stdoutput, буфер, len, &n, nullptr))
		выведиОш(-1, "Couldn't пиши output to console.");
}

проц SshShell::ConsoleRawMode(бул b)
{
	if(!channel || mode != CONSOLE)
		return;

	if(b) {
		GetConsoleMode(stdinput, &tflags);
		DWORD nflags = tflags;
		nflags &= ~ENABLE_LINE_INPUT;
		nflags &= ~ENABLE_ECHO_INPUT;
		nflags |= ENABLE_WINDOW_INPUT;
		SetConsoleMode(stdinput, nflags);
	}
	else
	if(rawmode)
		SetConsoleMode(stdinput, tflags);
	rawmode = b;
}

размер SshShell::GetConsolePageSize()
{
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	обнули(cinf);
	if(GetConsoleScreenBufferInfo((HANDLE) _get_osfhandle(1), &cinf))
		return размер(cinf.dwSize.X, cinf.dwSize.Y);
	LLOG("Warning: Couldn't read local terminal page size.");
	return Null;
}

#endif

бул SshShell::х11Иниц()
{
	if(!xenabled)
		return true;

	return Ssh::пуск([=]() mutable {
#ifdef PLATFORM_POSIX
		цел rc = libssh2_channel_x11_req(*channel, xscreen);
		if(!WouldBlock(rc) && rc < 0)
			выведиОш(rc);
		if(!rc)
			LLOG("X11 tunnel succesfully initialized.");
		return !rc;
#elif PLATFORM_WIN32
		выведиОш(-1, "X11 tunneling is not (yet) supported on Windows platform");
		return false;
#endif
	});
}

проц SshShell::х11Цикл()
{
#ifdef PLATFORM_POSIX
	if(xrequests.пустой())
		return;

	for(цел i = 0; i < xrequests.дайСчёт(); i++) {
		SshX11Handle xhandle = xrequests[i].a;
		SOCKET sock = xrequests[i].b;

		if(СобытиеWait(sock, WAIT_WRITE, 0)) {
			цел rc = static_cast<цел>(
				libssh2_channel_read(xhandle, xbuffer, т_мера(xbuflen))
				);
			if(!WouldBlock(rc) && rc < 0)
				выведиОш(-1, "[X11]: читай failed.");
			if(rc > 0)
				write(sock, xbuffer, rc);
		}
		if(СобытиеWait(sock, WAIT_READ, 0)) {
			цел rc =  static_cast<цел>(
				read(sock, xbuffer, т_мера(xbuflen))
				);
			if(rc > 0)
				libssh2_channel_write(xhandle, (const сим*) xbuffer, т_мера(rc));
		}
		if(libssh2_channel_eof(xhandle) == 1) {
			LLOG("[X11] EOF received.");
			close(sock);
			xrequests.удали(i);
			i = 0;
		}
	}
#endif
}

SshShell& SshShell::ForwardX11(const Ткст& host, цел дисплей, цел screen, цел bufsize)
{
	if(!xenabled) {
		xenabled = true;
#ifdef PLATFORM_POSIX
		xhost    = host;
		xdisplay = дисплей;
		xscreen  = screen;
		xbuflen  = clamp(bufsize, ssh->chunk_size, INT_MAX);
		xbuffer.размести(xbuflen);
#endif
	}
	return *this;
}

бул SshShell::AcceptX11(SshX11Handle xhandle)
{
#ifdef PLATFORM_POSIX
	if(xhandle && xenabled) {
		auto sock = socket(AF_UNIX, SOCK_STREAM, 0);
		if(sock < 0) {
			LLOG("Couldn't create UNIX socket.");
			return false;
		}
		auto path = фмт("%s/.X11-unix/X%d", дайВремПуть(), xdisplay);

		struct sockaddr_un addr;
		обнули(addr);
		addr.sun_family = AF_UNIX;
		memcpy(addr.sun_path, ~path, path.дайДлину());

		if(connect(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
			LLOG("Couldn't connect to " << path);
			close(sock);
			return false;
		}

		LLOG("X11 connection accepted.");

		auto& xr = xrequests.добавь();
		xr.a = xhandle;
		xr.b = sock;
		return true;
	}
#endif
	return false;
}


SshShell::SshShell(SshSession& session)
: SshChannel(session)
, mode(GENERIC)
, rawmode(false)
, resized(false)
, xenabled(false)
#ifdef PLATFORM_POSIX
, xdisplay(0)
, xscreen(0)
, xbuflen(1024 * 1024)
#elif PLATFORM_WIN32
, stdinput(nullptr)
, stdoutput(nullptr)
#endif
{
    ssh->otype = SHELL;
	обнули(tflags);
}

SshShell::~SshShell()
{
	ConsoleRawMode(false);

}
