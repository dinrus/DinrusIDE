#include "SqlCtrl.h"
#include "SqlDlg.h"
#include <Report/Report.h>
#include <CodeEditor/CodeEditor.h>

namespace Upp {

#define IMAGEFILE <SqlCtrl/SqlCtrl.iml>
#define IMAGECLASS SqlConsoleImg
#include <Draw/iml.h>

static bool scmpw(const byte *s, const char *w) {
	for(;;) {
		if(*w == '\0')
			return *s == ' ' || *s == '\t';
		if(ToUpper(*s++) != ToUpper(*w++))
			return false;
	}
}

static const int MAX_NESTING_LEVEL = 10;
static const int PROGRESS_RANGE = 1000000000;

bool SqlRunScript(int dialect, Stream& script_stream, 
	Gate<String> executor, Gate<int, int> progress,
	Index<String>& script_file_names, int progress_min, int progress_max)
{
	int line_number = 1;
	for(int c; (c = script_stream.Term()) >= 0;) {
		if(c <= ' ') {
			if(c == '\n')
				line_number++;
			script_stream.Get();
		}
		else if(c == '@') {
			script_stream.Get();
			int64 start = script_stream.GetPos() - 1;
			bool rel = false;
			if(script_stream.Term() == '@') {
				script_stream.Get();
				rel = true;
			}
			StringBuffer subscript_fn;
			while((c = script_stream.Get()) >= 0 && c != '\n')
				subscript_fn.Cat((char)c);
			int e = subscript_fn.GetLength();
			while(e > 0 && subscript_fn[e - 1] <= ' ')
				e--;
			String fn(~subscript_fn, e);
			fn = NormalizePath(fn, GetFileDirectory(script_file_names[rel ? script_file_names.GetCount() - 1 : 0]));
			String norm = fn;
#if !PLATFORM_PATH_HAS_CASE
			norm = ToLower(norm);
#endif
			if(script_file_names.Find(norm) >= 0)
				throw Exc(NFormat(t_("циркулярное включение сценария %s в %s:%d"),
					fn, script_file_names.Top(), line_number));
			if(script_file_names.GetCount() >= MAX_NESTING_LEVEL)
				throw Exc(NFormat(t_("слишком глубокое гнездование сценария (%d уровней)"), script_file_names.GetCount()));
			int64 end = script_stream.GetPos();
			
			int new_min = progress_min + (int)((progress_max - progress_min) * start / script_stream.GetSize());
			int new_max = progress_min + (int)((progress_max - progress_min) * end / script_stream.GetSize());
			
			FileIn fi;
			if(!fi.Open(fn))
				throw Exc(NFormat(t_("ошибка при открытии файла сценария '%s'"), fn));
			
			script_file_names.Add(norm);
			if(!SqlRunScript(dialect, fi, executor, progress, script_file_names, new_min, new_max))
				return false;
			script_file_names.Drop();
		}
		else {
			StringBuffer statement;
			bool exec = false;
			bool body = false;
			bool chr = false;

			while(!script_stream.IsEof() && !exec) {
				String line = script_stream.GetLine();
				line_number++;
				
				int l = line.GetLength();
				if(l && line[0] == '/' && !body && !chr)
					exec = true;
				else
				if(l && line[0] == '.' && body && !chr)
					body = false;
				else {
					if(statement.GetLength() && !chr)
						statement.Cat(' ');
					bool spc = true;
					bool create = false;
					for(const byte *s = (const byte *)(const char *)line; *s; s++) {
						if(s[0] == '-' && s[1] == '-' && !chr) break;
						if(*s == '\'') chr = !chr;
						if(!chr && spc && scmpw(s, "create"))
							create = true;
						if(!chr && spc && (scmpw(s, "begin") || scmpw(s, "declare") ||
						   create && (scmpw(s, "procedure") || scmpw(s, "function") || scmpw(s, "trigger"))
						))
							body = true;
						if(*s == ';' && !chr && !body) {
							exec = true;
							break;
						}
						if(*s > ' ' || chr) {
							statement.Cat(*s);
							spc = false;
						}
						else
						if(*s == '\t' || *s == ' ') {
							if(!spc) statement.Cat(' ');
							spc = true;
						}
						else
							spc = false;
					}
					if(chr) statement.Cat("\r\n");
				}
			}
			if(progress((int)(progress_min + script_stream.GetPos()
				* (progress_max - progress_min) / script_stream.GetSize()), PROGRESS_RANGE))
				throw AbortExc();
			if(!executor(statement))
				return false;
		}
	}
	return true;
}

bool SqlRunScript(int dialect, Stream& script_stream, const String& file_name,
                  Gate<String> executor, Gate<int, int> progress)
{
	Index<String> script_file_names;
	String fn = NormalizePath(file_name);
#if !PLATFORM_PASH_HAS_CASE
	fn = ToLower(fn);
#endif
	script_file_names.Add(fn);
	return SqlRunScript(dialect, script_stream, executor, progress, script_file_names, 0, PROGRESS_RANGE);
}

void RunDlgSqlExport(Sql& cursor, String command, String tablename);

class SqlConsole : public TopWindow {
public:
	virtual bool Key(dword key, int count);
	virtual void Serialize(Stream& s);

	void ListMenu(Bar& bar);
	void ListPrintRow();
	void ListPrintList();
	void ListExport();

protected:
	Sql                        cursor;
	String                     lastquery;
	Splitter                   lires, vsplit;
	ArrayCtrl                  list;
	StaticRect                 rec_err;
	DocEdit                    errortext;
	ArrayCtrl                  record;
	ArrayCtrl                  trace;
	CodeEditor                 command;
	Button                     execute;
	Button                     schema;
	Button                     csv;
	ParentCtrl                 command_pane;
//	CallbackSet                hide;
	Vector<int>                cw;
	Vector<bool>               visible;
	Vector<bool>               lob;
	String                     LastDir;
	Label                      info1, info2;

	enum {
		NORMAL,
		RERUN,
		SCRIPT,
		QUIET,
	};

	void    ColSize();
	void    Hide(int i);
	void    Record();
	void    Execute(int type = NORMAL);
	void    TraceToCommand();
	void    TraceToExecute();
	void    ListToCommand(ArrayCtrl *list);
	void    SaveTrace();
	void    RunScript(bool quiet);
	void    TraceMenu(Bar& menu);
	void    ObjectTree() { SQLObjectTree(cursor.GetSession()); }
	void    Csv();

	class Exec;
	friend class Exec;
	class Exec : public StatementExecutor {
	public:
		typedef Exec CLASSNAME;
		Exec(bool quiet) : quiet(quiet) {}
		SqlConsole *me;
		bool quiet;
		virtual bool Execute(const String& stmt) {
			me->command <<= stmt; me->Sync(); me->Execute(quiet ? QUIET : SCRIPT); return true;
		}
		bool GateExec(String stmt) { return Execute(stmt); }
		operator Gate<String> ()   { return THISBACK(GateExec); }
		
	};

	void ViewRecord();

public:
	typedef SqlConsole CLASSNAME;

	void    Perform();

	void    AddInfo(Ctrl& tgt, Label& info, const char *txt);

	SqlConsole(SqlSession& session);
};

void SqlConsole::Execute(int type) {
	list.Reset();
	list.HeaderObject().Absolute().Moving();
	visible.Clear();
	lob.Clear();
	record.Clear();
	String s = ~command;
	while(*s.Last() == ';')
		s.Trim(s.GetLength() - 1);
	int ms0 = msecs();
	cursor.ClearError();
	lastquery = s;
	String ttl = s;
	ttl.Replace("\t", " ");
	ttl.Replace("\n", " ");
	ttl.Replace("\r", "");
	info1.Remove();
	info2.Remove();
	if(!cursor.Execute(s)) {
	error:
		record.Hide();
		errortext.Show();
		list.AddColumn(t_("Ошибка"));
		String err = cursor.GetLastError();
		errortext <<= err;
		list.Add(err);
		trace.Add(s, err, "");
		trace.GoEnd();
		Title((ttl + " - " + err).ToWString());
		return;
	}
	if(type == QUIET)
		return;
	bool onecol = cursor.GetColumns() == 1;
	record.Show(!onecol);
	errortext.Show(onecol);
	int ms1 = msecs();
	cw.SetCount(cursor.GetColumns());
	visible.SetCount(cw.GetCount(), true);
	int margins;
	for(int i = 0; i < cursor.GetColumns(); i++) {
		const SqlColumnInfo& ci = cursor.GetColumnInfo(i);
		String n = ToLower(ci.name);
		list.AddColumn(n);
		list.HeaderTab(i).WhenAction = THISBACK1(Hide, i);
		margins = HorzLayoutZoom(2) + 2 * list.HeaderTab(i).GetMargin();
		cw[i] = GetTextSize(n, StdFont()).cx + margins;
		record.Add(n, Null);
		lob.Add(ci.type == -1 || ci.type == -2); // !! BLOB / CLOB hack
	}
	Progress pi;
	pi.SetText(t_("Фетчировано %d строк(а)"));
	while(cursor.Fetch()) {
		Vector<Value> row = cursor.GetRow();
		for(int i = 0; i < cursor.GetColumns(); i++)
		{
			if(lob[i])
			{
				String temp;
				cursor.GetColumn(i, temp);
				row[i] = temp;
			}
			cw[i] = max(cw[i], GetTextSize(StdFormat(row[i]), StdFont()).cx + margins);
			cw[i] = min(cw[i], list.GetSize().cx / 3);
		}
		list.Add(row);
		if(pi.StepCanceled()) break;
	}
	if(cw.GetCount() && cursor.WasError()) {
		list.Reset();
		goto error;
	}
	visible.SetCount(list.GetColumnCount(), true);
	ColSize();
	if(list.GetCount() > 0)
		list.SetCursor(0);
	Title(NFormat(t_("%s (%d рядов)"), ttl, list.GetCount()));
	String rrows = Format(t_("%d рядов"), max(list.GetCount(), cursor.GetRowsProcessed()));
	String rms = Format(t_("%d мс"), ms1 - ms0);
	if(type == RERUN && trace.IsCursor()) {
		trace.Set(1, rrows);
		trace.Set(2, rms);
	}
	else {
		trace.Add(s, rrows, rms);
		trace.GoEnd();
	}
	command.Remove(0, command.GetLength());
	command.SetSelection(0, 0);
}

void SqlConsole::ColSize() {
	int maxw = list.GetSize().cx;
	int wx = 0;
	for(int i = 0; i < list.GetColumnCount(); i++)
		if(visible[i]) {
			int w = min(maxw, cw[i]);
			wx += w;
			list.HeaderObject().SetTabRatio(i, w);
			list.HeaderObject().ShowTab(i);
		}
		else
			list.HeaderObject().HideTab(i);
}

void SqlConsole::Hide(int i) {
	if(i < visible.GetCount())
		visible[i] = false;
	ColSize();
}

void SqlConsole::Record() {
	if(list.GetIndexCount() == 1)
		errortext <<= StdFormat(list.Get(0));
	for(int i = 0; i < list.GetIndexCount(); i++)
		record.Set(i, 1, list.IsCursor() ? list.Get(i) : Value());
}

bool SqlConsole::Key(dword key, int count) {
	switch(key) {
	case K_F5:
		Execute();
		return true;
	case K_CTRL_R:
		RunScript(false);
		return true;
	case K_CTRL_Q:
		RunScript(true);
		return true;
	case K_CTRL_S:
		SaveTrace();
		return true;
	}
	return TopWindow::Key(key, count);
}

void SqlConsole::Serialize(Stream& s) {
	int version = 0;
	s / version;
	s.Magic();
	Rect r = GetRect();
	s % r;
	SetRect(r);
	vsplit.Serialize(s);
	record.SerializeHeader(s);
	lires.Serialize(s);
	trace.SerializeHeader(s);
	if(s.IsLoading())
		trace.Clear();
	Vector<ValueArray> ar;
	for(int i = 0; i < trace.GetCount(); i++)
		ar.Add(trace.GetArray(i));
	s % ar;
	if(s.IsLoading()) {
		for(int i = 0; i < ar.GetCount(); i++)
			trace.SetArray(i, ar[i]);
		trace.GoEnd();
	}
	if(version >= 1)
		s % LastDir;
	s.Magic();
}

void SqlConsole::Perform() {
	const char cfg[] = "SqlConsole.cfg";
	LoadFromFile(*this, cfg);
	Title(t_("Коммандер SQL"));
	Icon(SqlConsoleImg::database_edit(), SqlConsoleImg::SqlConsoleIconLarge());
	Sizeable();
	Zoomable();
	ActiveFocus(command);
	Run();
	cursor.ClearError();
	StoreToFile(*this, cfg);
}

void SqlConsole::TraceToCommand() {
	if(trace.IsCursor()) {
		command.SetData(trace.Get(0));
		command.SetCursor(command.GetLength());
	}
}

void SqlConsole::ListToCommand(ArrayCtrl *l)
{
	int c = l->GetClickColumn();
	if(GetCtrl() && l->IsCursor() && c >= 0 && c < l->GetColumnCount()) {
		command.Paste(AsString(l->Get(c)).ToWString());
		command.SetFocus();
	}
}

void SqlConsole::TraceToExecute() {
	Execute(RERUN);
}

void SqlConsole::SaveTrace() {
	FileSel fsel;
	fsel.ActiveDir(LastDir);
	fsel.DefaultExt("sql");
	fsel.Type(t_("Сценарии SQL (*.sql)"), "*.sql");
	fsel.AllFilesType();
	if(!fsel.ExecuteSaveAs(t_("Сохранить трассировку как"))) return;
	FileOut out(~fsel);
	if(!out) return;
	LastDir = GetFileDirectory(~fsel);
	for(int i = 0; i < trace.GetCount(); i++) {
		out.Put((String)trace.Get(i, 0));
		out.Put(";\n");
	}
}

void SqlConsole::RunScript(bool quiet) {
//	UPP::RunScript runscript = cursor.GetSession().GetRunScript();
//	if(!runscript) {
//		Exclamation(t_("Database connection doesn't support running scripts."));
//		return;
//	}
	FileSel fsel;
	fsel.ActiveDir(LastDir);
	fsel.DefaultExt("sql");
	fsel.Type(t_("Сценарии SQL (*.sql)"), "*.sql");
	fsel.AllFilesType();
	if(!fsel.ExecuteOpen(t_("Выполнить сценарий"))) return;
	Exec exec(quiet);
	exec.me = this;
	LastDir = GetFileDirectory(~fsel);
	Progress progress(t_("Выполняется сценарий"));
	FileIn fi;
	if(!fi.Open(~fsel)) {
		Exclamation(NFormat(t_("Не уда1тся открыть файл [* \1%s\1]."), ~fsel));
		return;
	}
	try {
		SqlRunScript(cursor.GetDialect(), fi, ~fsel, exec, progress);
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void SqlConsole::TraceMenu(Bar& menu) {
	menu.Add(t_("Сохранить как сценарий.."), THISBACK(SaveTrace)).Key(K_CTRL_S);
	menu.Add(t_("Выполнить сценарий.."), THISBACK1(RunScript, false)).Key(K_CTRL_R);
	menu.Add(t_("Выполнить сценарий втихую.."), THISBACK1(RunScript, true)).Key(K_CTRL_Q);
}

void SqlConsole::ListMenu(Bar& bar)
{
	bar.Add(t_("Печать записи"), THISBACK(ListPrintRow));
	bar.Add(t_("Печать списка"), THISBACK(ListPrintList));
	bar.Add(t_("Экспорт..."), THISBACK(ListExport));
}

void SqlConsole::Csv()
{
	SelectSaveFile("Csv\t*.csv", list.AsCsv());
}

void SqlConsole::ListPrintRow()
{
	String qtf;
	qtf << "[A1 ";
	if(!IsNull(lastquery))
		qtf << "[4* \1" << lastquery << "\1]&&";
	for(int i = 0; i < record.GetCount(); i++) {
		qtf << (i ? "--" : "++") << "::10@(240.240.240) \1" << StdFormat(record.Get(i, 0)) << "\1"
			"||::30@(255.255.255) \1" << StdFormat(record.Get(i, 1)) << "\1";
	}
	qtf << "++";
	Report report;
	report << qtf;
	UPP::Perform(report);
}

void SqlConsole::ListPrintList()
{
	String qtf;
	qtf << "[A1 ";
	if(!IsNull(lastquery))
		qtf << "[4* \1" << lastquery << "\1]&&";
	for(int i = 0; i < record.GetCount(); i++)
		qtf << (i ? "||" : "++") << "::@(240.240.240) [* \1"
		<< StdFormat(record.Get(i, 0)) << "\1]";
	for(int i = 0; i < list.GetCount(); i++)
		for(int j = 0; j < list.GetIndexCount(); j++)
			qtf << (j ? "||" : "--") << "::@(255.255.255) \1" << StdFormat(list.Get(i, j)) << '\1';
	qtf << "++";
	Report report;
	report << qtf;
	UPP::Perform(report);
}

void SqlConsole::ListExport()
{
	RunDlgSqlExport(cursor, lastquery, Null);
}

struct SqlValueViewDlg : public WithSqlValueViewLayout<TopWindow> {
	typedef SqlValueViewDlg CLASSNAME;
	
	String value;
	
	void Sync();
	void Save();

	SqlValueViewDlg();
};

void SqlValueViewDlg::Sync()
{
	if(~format) {
		StringStream ss;
		HexDumpData(ss, ~value, value.GetLength(), false, 1000000);
		text <<= ss.GetResult();
	}		
	else
		text <<= value;
}

void SqlValueViewDlg::Save()
{
	SelectSaveFile("Файл\t*.*", value);
}

SqlValueViewDlg::SqlValueViewDlg()
{
	CtrlLayout(*this, "");
	text.SetFont(Monospace(GetStdFont().GetHeight()));
	format <<= THISBACK(Sync);
	save <<= THISBACK(Save);
	format = 0;
}

void SqlViewValue(const String& title, const String& value)
{
	SqlValueViewDlg dlg;
	dlg.Title(title);
	dlg.value = value;
	dlg.Sync();
	dlg.Execute();
}

void SqlConsole::ViewRecord()
{
	if(!record.IsCursor())
		return;
	SqlViewValue(record.Get(0), AsString(record.Get(1)));
}

void SqlConsole::AddInfo(Ctrl& tgt, Label& info, const char *txt)
{
	info = txt;
	tgt.Add(info.SizePos());
//	tgt.Add(info.BottomPos(0, GetStdFontCy()).RightPos(0, GetTextSize(txt, StdFont()).cx));
}

SqlConsole::SqlConsole(SqlSession& session)
: cursor(session)
{
	int ecy = EditField::GetStdHeight();
	lires.Horz(list, rec_err);
	rec_err << record.SizePos() << errortext.SizePos();
	errortext.SetReadOnly();
	errortext.SetFont(Courier(12));
	errortext.Hide();
	vsplit.Vert(lires, trace);
	vsplit.Add(command_pane);
	vsplit.SetPos(6500);
	vsplit.SetPos(8500, 1);
	lires.SetPos(7000);
	record.AddColumn(t_("Графа"), 5);
	record.AddColumn(t_("Значение"), 10);
	record.WhenLeftDouble = THISBACK(ViewRecord);
	record.WhenLeftClick = THISBACK1(ListToCommand, &record);
	AddInfo(record, info1, "\1[g= Используйте [@B Ctrl+Click], чтобы копировать данные в SQL&[@B DoubleClick] для детального обзора.");
	trace.AddColumn(t_("Команда"), 8);
	trace.AddColumn(t_("Результат"), 1);
	trace.AddColumn(t_("Продолжительность"), 1);
	trace.WhenLeftClick = THISBACK(TraceToCommand);
	trace.WhenLeftDouble = THISBACK(TraceToExecute);
	trace.WhenBar = THISBACK(TraceMenu);
	trace.NoWantFocus();
	list.WhenSel = THISBACK(Record);
	list.WhenLeftClick = THISBACK1(ListToCommand, &list);
	list.WhenBar = THISBACK(ListMenu);
	list.HeaderObject().Absolute();
	AddInfo(list, info2, "\1[g= Используйте [@B Ctrl+Click], чтобы копировать данные в SQL.");
	Add(vsplit.SizePos());
	command.SetFont(Courier(GetStdFontCy()));
	command_pane.Add(command.VSizePos().HSizePos(0, HorzLayoutZoom(100)));
	ecy = max(24, ecy);
	command_pane.Add(execute.TopPos(0, ecy).RightPos(4, HorzLayoutZoom(90)));
	command_pane.Add(schema.TopPos(ecy + 4, ecy).RightPos(4, HorzLayoutZoom(90)));
	command_pane.Add(csv.TopPos(2 * ecy + 4, ecy).RightPos(4, HorzLayoutZoom(90)));
	command.Highlight("sql");
	schema.SetLabel(t_("&Схема"));
	schema <<= THISBACK(ObjectTree);
	schema.SetImage(SqlConsoleImg::bricks());
	execute <<= THISBACK1(Execute, NORMAL);
	execute.SetImage(SqlConsoleImg::lightning());
	execute.SetLabel(t_("Выполнить (F5)"));
	csv <<= THISBACK(Csv);
	csv.SetLabel(t_("Экспорт.."));
	csv.SetImage(SqlConsoleImg::database_save());
	ActiveFocus(command);
}

void SQLCommander(SqlSession& session) {
	SqlConsole con(session);
	con.Perform();
}

bool IsSqlConsoleActive__()
{
	return dynamic_cast<SqlConsole *>(Ctrl::GetActiveWindow());
}

}
