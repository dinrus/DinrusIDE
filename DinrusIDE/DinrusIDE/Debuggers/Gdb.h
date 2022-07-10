class Gdb
	: public Отладчик
	, public КтрлРодитель
{
	typedef Gdb ИМЯ_КЛАССА;
	
public:
	virtual void DebugBar(Бар& bar) override;
	virtual bool SetBreakpoint(const Ткст& filename, int line, const Ткст& bp) override;
	virtual bool RunTo() override;
	virtual void пуск() override;
	virtual bool Ключ(dword ключ, int count) override;
	virtual void стоп() override;
	virtual bool окончен() override;
	virtual bool Подсказка(const Ткст& exp, РедакторКода::MouseTip& mt) override;

	void покажиИскл();

public:
	Ткст DoRun();
	
	bool результат(Ткст& result, const Ткст& s);

	void      AddReg(const char *reg, Надпись *lbl) { regname.добавь(reg); reglbl.добавь(lbl); }

	void      блокируй();
	void      разблокируй();

	void      SyncFrameButtons();

	Ткст    Cmd(const char *command, bool start = false);
	Ткст    FastCmd(const char *command);
	
	bool      IsProcessExitedNormally(const Ткст& cmd_output) const;
	Ткст    ObtainThreadsInfo();
	
	bool      TryBreak(const char *command);
	void      CheckEnd(const char *result);

	void      Step(const char *cmd);

	Ткст    Cmdp(const char *cmdline, bool fr = false, bool setframe = true);

	void      DoRunTo() { RunTo(); }

	void      SetDisas(const Ткст& text);
	void      SyncDisas(bool fr);

	void      DisasCursor();
	void      DisasFocus();
	void      DropFrames();
	void      LoadFrames();
	
	void      SwitchFrame();
	void      FrameUpDown(int dir);
	void      SwitchThread();
	
	void      ClearCtrls();
	
	static void ReadGdbValues(СиПарсер& p, ВекторМап<Ткст, Ткст>& val);
	static void ReadGdbValues(const Ткст& h, ВекторМап<Ткст, Ткст>& val);

	Ткст    Print0(const Ткст& exp);
	Ткст    Print(const Ткст& exp);
	void      Locals();
	void      Watches();
	void      TryAuto(Индекс<Ткст>& tested, const Ткст& exp);
	void      Autos();
	void      ObtainData();
	void      Self();
	void      Cpu();
	void      ClearWatches();
	void      QuickWatch();
	void      SetTab(int q);
	void      SetTree(КтрлМассив *a);
	void      OnTreeBar(Бар& bar);
	void      WatchMenu(Бар& bar);
	void      OnTreeExpand(int node);
	void      MemoryGoto();
	void      MemoryLoad(const Ткст& adr, int count, bool showerror);
	void      Memory();
	void      MemoryMenu(Бар& bar, const Ткст& exp);

	void      OnValueCopyToClipboard(const Значение& val);
	
	Значение     ObtainValueFromTreeCursor(int cursor) const;
	
	void      копируйСтэк();
	void      копируйВесьСтэк();
	void      копируйДизас();
	
	void      BreakRunning();
	
	bool      создай(Хост& host, const Ткст& exefile, const Ткст& cmdline, bool console);
	
	ОбрвызВремени periodic; // Period check for killed console
	void Periodic();

	void SerializeSession(Поток& s);
	
	virtual ~Gdb() override;
	Gdb();
	
protected:
	ЛокальнПроцесс       dbg;

	Вектор<Ткст>     regname;
	Вектор<Надпись *>    reglbl;

	DbgDisas           disas;

	bool               nodebuginfo = false;
	СтатичПрям         nodebuginfo_bg;
	СтатичТекст         nodebuginfo_text;

	EditString         watchedit;
	СписокБроса           threads;
	СписокБроса           frame;
	Кнопка             frame_up, frame_down;
	КтрлВкладка            tab;
	КтрлМассив          locals;
	КтрлМассив          watches;
	КтрлМассив          autos;
	КтрлМассив          self;
	СписокКолонок         cpu;
	Надпись              dlock;
	СтатичПрям         pane;
	Сплиттер           split;
	КтрлДерево           tree;
	struct GdbHexView : ГексОбзор {
		uint64       start;
		Вектор<byte> data;

		virtual int Байт(int64 i)            { return i >= 0 && i < data.дайСчёт() ? data[(int)i] : 0; }
	}
	memory;

	ВекторМап<Ткст, Ткст> expression_cache;

	WithQuickwatchLayout<ТопОкно> quickwatch;

	Ткст             file;
	int                line;
	adr_t              addr;

	ФайлВывод            log;
	
	Ткст             autoline;
	bool               firstrun;
	bool               running_interrupted;
	
	int                pid = 0; // debugee pid

	Ткст             bp_filename;
	int                bp_line;
	Ткст             bp_val;
	
	Ткст             exception;
	
	const int max_stack_trace_size = 400;
	
	Один<IGdbUtils> gdb_utils;
};
