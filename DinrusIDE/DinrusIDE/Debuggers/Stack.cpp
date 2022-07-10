#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x)  // DLOG(x)

Pdb::Нить& Pdb::Current()
{
	int q = threads.найди((int)~threadlist);
	if(q >= 0)
		return threads[q];

	static Нить zero;
	return zero;
}

Массив<Pdb::Фрейм> Pdb::Backtrace(Нить& ctx, bool single_frame, bool thread_info)
{
	Массив<Фрейм> frame;
	STACKFRAME64 stackFrame = {0};
	stackFrame.AddrPC.Mode = stackFrame.AddrFrame.Mode = stackFrame.AddrStack.Mode = AddrModeFlat;
	DWORD machineType = IMAGE_FILE_MACHINE_I386;
	void *c;
	Нить cctx = ctx;
#ifdef CPU_64
	if(win64) {
		machineType = IMAGE_FILE_MACHINE_AMD64;
		stackFrame.AddrPC.Offset = ctx.context64.Rip;
		stackFrame.AddrFrame.Offset = ctx.context64.Rbp;
		stackFrame.AddrStack.Offset = ctx.context64.Rsp;
		c = &cctx.context64;
	}
	else
#endif
	{
		machineType = IMAGE_FILE_MACHINE_I386;
		stackFrame.AddrPC.Offset = ctx.context32.Eip;
		stackFrame.AddrFrame.Offset = ctx.context32.Ebp;
		stackFrame.AddrStack.Offset = ctx.context32.Esp;
		c = &cctx.context32;
	}

	DWORD64 lastFrame = 0; // Prevent loops with optimised stackframes
	while(::StackWalk64(machineType, hProcess, ctx.hThread, &stackFrame, c,
	      0, ::SymFunctionTableAccess64, ::SymGetModuleBase64, 0)) {
	    if(stackFrame.AddrPC.Offset == 0 || lastFrame >= stackFrame.AddrFrame.Offset) {
			LLOG("Invalid stack frame");
			break;
	    }
		lastFrame = stackFrame.AddrFrame.Offset;
		LLOG("PC: " << Гекс(stackFrame.AddrPC.Offset));
		Фрейм& f = frame.добавь();
		f.pc = stackFrame.AddrPC.Offset;
		f.frame = stackFrame.AddrFrame.Offset;
		f.stack = stackFrame.AddrStack.Offset;
		f.фн = GetFnInfo(f.pc);
		if(thread_info) {
			if(frame.дайСчёт() > 20)
				break;
		}
		else {
			if(пусто_ли(f.фн.имя)) {
				if(single_frame)
					return frame;
				f.text = Гекс(f.pc);
				for(int i = 0; i < module.дайСчёт(); i++) {
					const ИнфОМодуле& m = module[i];
					if(f.pc >= m.base && f.pc < m.base + m.size) {
						f.text << " (" << дайИмяф(m.path) << ")";
						break;
					}
				}
			}
			else {
				GetLocals(f, cctx, f.param, f.local);
				if(single_frame)
					return frame;
				f.text = f.фн.имя;
				f.text << '(';
				for(int i = 0; i < f.param.дайСчёт(); i++) {
					if(i)
						f.text << ", ";
					f.text << f.param.дайКлюч(i) << "=" << Visualise(f.param[i], MEMBER).дайТкст();
				}
				f.text << ')';
			}
		}
	}
	return frame;
}

void Pdb::Sync0(Нить& ctx)
{
	stop = false;

	framelist.очисть();
	frame.очисть();
	current_frame = NULL;

	frame = Backtrace(ctx);
	for(const Фрейм& f : frame)
		framelist.добавь(framelist.дайСчёт(), f.text);
	framelist.идиВНач();
}

void Pdb::BTs()
{
	static bool lock;
	
	if(lock) return;
	
	int cid = bts.дайКурсор();
	Значение cursor = bts.дай();
	Точка sc = bts.дайПромотку();

	Индекс<Ткст> open;
	for(int i = 0; i < bts.GetChildCount(0); i++) {
		int id = bts.GetChild(0, i);
		if(bts.открыт(id))
			open.найдиДобавь(~bts.дай(id));
	}
	
	bts.очисть();
	bts.NoRoot();

	bts.WhenOpen = [=](int id) {
		if(bts.GetChildCount(id) == 0) {
			Ткст thid = ~bts.дай(id);
			int ii = threads.найди(atoi(thid));
			if(ii >= 0) {
				int i = 0;
				for(const auto& f : Backtrace(threads[ii])) {
					bts.добавь(id, i == 0 ? DbgImg::IpLinePtr() : DbgImg::FrameLinePtr(),
					        "#" + какТкст(i) + "#" + thid, f.text);
					i++;
				}
			}
		}
	};

	for(int i = 0; i < threads.дайСчёт(); i++) {
		dword thid = threads.дайКлюч(i);
		Ткст stid = какТкст(thid);
		Ткст info;
		bool waiting = false;
		bool cowork = false;
		for(const Фрейм& f : Backtrace(threads[i], false, true)) {
			if(info.дайСчёт() + f.фн.имя.дайСчёт() > 160) {
				info << "...";
				break;
			}
			bool zwait = f.фн.имя.начинаетсяС("ZwWait");
			waiting |= zwait;
			if(!f.фн.имя.начинаетсяС("RtlSleep") && !f.фн.имя.начинаетсяС("ZwWaitForAlertByThreadId"))
				MergeWith(info, ", ", f.фн.имя);
			if(f.фн.имя == "РНЦП::СоРабота::Pool::ThreadRun") {
				cowork = true;
				break;
			}
		}
		int id = bts.добавь(0, DbgImg::Thread(), stid,
		                 AttrText(фмт("0x%x ", (int)thid) + info)
		                 .NormalInk(waiting ? cowork ? смешай(SLtBlue, SGray(), 200) : SGray()
		                                    : cowork ? SLtBlue() : SColorText()),
		                 true);
		if(open.найди(stid) >= 0)
			bts.открой(id);
	}
	
	bts.FindSetCursor(cursor);
	bts.ScrollTo(sc);
	
	bts.WhenBar = [=](Бар& bar) {
		bar.добавь("открой All", [=] {
			for(int i = 0; i < bts.GetChildCount(0); i++)
				bts.открой(bts.GetChild(0, i));
		});
		bar.добавь("закрой All", [=] {
			for(int i = 0; i < bts.GetChildCount(0); i++)
				bts.закрой(bts.GetChild(0, i));
		});
	};
	
	bts.WhenSel = [=] {
		Ткст k = ~bts.дай();
		if(*k == '#') {
			int id = bts.дайРодителя(bts.дайКурсор());
			Ткст thid = ~bts.дай(id);
			int tid = atoi(thid);
			int ii = threads.найди(tid);
			if(ii >= 0) {
				int i = 0;
				for(auto& f : Backtrace(threads[ii])) {
					if(k == "#" + какТкст(i) + "#" + thid) {
						int tid = atoi(thid);
						if(threadlist.HasKey(tid)) {
							lock = true;
							threadlist <<= tid;
							устНить();
							if(ii < framelist.дайСчёт()) {
								framelist.SetIndex(i);
								устФрейм();
								lock = false;
								return;
							}
							lock = false;
						}
					}
					i++;
				}
			}
		}
	};
}

#endif
